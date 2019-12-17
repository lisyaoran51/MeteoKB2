#ifndef MAP_ALGORITHM_H
#define MAP_ALGORITHM_H

#include "../../../../../Framework/Graphic/Map/Map.h"
#include "../../../../../Framework/Allocation/Hierachal/Container.h"
#include "MapGenerateAlgorithm.h"
#include "MapShiftAlgorithm.h"
#include "../../../../../Framework/Configurations/FrameworkConfigManager.h"


using namespace Framework::Graphic::Maps;
using namespace Util;
using namespace Framework::Configurations;
using namespace Framework::Allocation::Hierachal;



namespace Games {
namespace Schedulers {
namespace Events {
namespace Effects {

	template<typename T>
	class EffectMapper;

}}}}

using namespace Games::Schedulers::Events::Effects;


namespace Games {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	

	class MapAlgorithmInterface: public Container {

	public:

		MapAlgorithmInterface(): Container(), RegisterType("MapAlgorithmInterface"){}

		virtual int RegisterBufferMap(Map* b) = 0;

		virtual int RegisterGenerator(MapGenerateAlgorithmInterface* g) = 0;

		virtual int RegisterShifter(MapShiftAlgorithmInterface* s) = 0;

		/// <summary>
		/// 把evnet的狀態轉成圖案，然後移到他的位置上
		///	</summary>
		virtual int Draw(Map* m, EventProcessor<Event>* em) = 0;

		virtual int SetStartX(int x) = 0;
	};


	template<typename T>
	class MapAlgorithm: public MapAlgorithmInterface
	{

		int load() {

			LOG(LogLevel::Info) << "MapAlgorithm::load() : Start loading config.";

			FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

			if (!f)
				throw runtime_error("int MapAlgorithm::load() : FrameworkConfigManager not found in cache.");

			return load(f);
		}

		int load(FrameworkConfigManager* f) {

			if (!f->Get(FrameworkSetting::StartPitch, &startX))
				throw runtime_error("int MapAlgorithm::load() : start pitch not found in FrameworkConfigManager.");

			if (!f->Get(FrameworkSetting::Width, &width))
				throw runtime_error("int MapAlgorithm::load() : width not found in FrameworkConfigManager.");

			if (!f->Get(FrameworkSetting::Height, &height))
				throw runtime_error("int MapAlgorithm::load() : hieght not found in FrameworkConfigManager.");

			return 0;
		}

	public:

		/// <summary>
		/// 無功用的建構子，要搭配lazy construct
		///	</summary>
		MapAlgorithm(): MapAlgorithmInterface(), RegisterType("MapAlgorithm") {
			registerLoad(bind((int(MapAlgorithm<T>::*)())&MapAlgorithm<T>::load, this));
		}

		~MapAlgorithm() {
			// TODO: ???
		}

		virtual int RegisterBufferMap(Map* b) {
			if (b->GetWidth() / 2 == width && b->GetHeight() / 2 == height)
			{
				bufferMap = b;
				return 0;
			}
			else
				throw invalid_argument("MapAlgorithm::RegisterBufferMap : Invalid size buffer map");
			// TODO: debug Log
			return -1;
		}

		virtual int RegisterGenerator(MapGenerateAlgorithmInterface* g) {
			if (CanCast<MapGenerateAlgorithm<T>>(g))
				return ImplementRegisterGenerator(Cast<MapGenerateAlgorithm<T>>(g));
			// throw error
			return -1;
		}

		virtual int RegisterShifter(MapShiftAlgorithmInterface* s) {
			if (CanCast<MapShiftAlgorithm<T>>(s))
				return ImplementRegisterShifter(Cast<MapShiftAlgorithm<T>>(s));
			// throw error
			return -1;
		}


		/// <summary>
		/// 把evnet的狀態轉成圖案，然後移到他的位置上
		///	</summary>
		//virtual int Draw(Map* m, T* e);

		/// <summary>
		/// 把evnet的狀態轉成圖案，然後移到他的位置上
		///	</summary>
		virtual int Draw(Map* m, EventProcessor<Event>* em) {
			// TODO: compiler should select if compile this line or not(debug level)
			//if (CanCast<EffectMapper<T>>(em))
			return ImplementDraw(m, Cast<EffectMapper<T>>(em));
			// throw error
			// return -1;
		}

		virtual int SetStartX(int x) {
			startX = x;
			if (shiftAlgo != nullptr)
				shiftAlgo->SetStartX(startX);
			return 0;
		}

	protected:

		int width;

		int height;
		
		int startX;
		//static Matrix2D<int>* matrix;

		Map* bufferMap;

		/// <summary>
		/// to transform the effect to the map with every parameter relevant.
		/// </summary>
		MapGenerateAlgorithm<T>* genAlgo = nullptr;

		/// <summary>
		/// 把生好的effect移到他該擺的位置上
		/// </summary>
		MapShiftAlgorithm<T>* shiftAlgo = nullptr;

		int ImplementRegisterGenerator(MapGenerateAlgorithm<T>* g) {
			genAlgo = g;
			return 0;
		}

		int ImplementRegisterShifter(MapShiftAlgorithm<T>* s) {
			shiftAlgo = s;
			return 0;
		}


		/// <summary>
		/// 把evnet的狀態轉成圖案，然後移到他的位置上
		///	</summary>
		//virtual int Draw(Map* m, T* e);

		/// <summary>
		/// 把evnet的狀態轉成圖案，然後移到他的位置上
		///	</summary>
		virtual int ImplementDraw(Map* m, EffectMapper<T>* em) {

			if (!bufferMap->IsClear())
				bufferMap->Reset();

			int returnValue = genAlgo->Generate(bufferMap, em);

			if (returnValue == -1)
				return 0;

			shiftAlgo->Shift(bufferMap, m, em);

			bufferMap->Reset();

			return 0;
		}

	};

	

}}}}}








#endif