#ifndef MAP_SHIFT_ALGORITHM_H
#define MAP_SHIFT_ALGORITHM_H

#include"../../../../Graphic/Map/Map.h"
#include "../../Event.h"
#include "../../EventProcessor.h"

using namespace Base::Graphic::Maps;

namespace Base {
namespace Schedulers {
namespace Events {
namespace Effects {

	template<typename T>
	class EffectMapper;

}}}}

using namespace Base::Schedulers::Events::Effects;
using namespace Base::Schedulers::Events;

namespace Base {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class MapShiftAlgorithmInterface {

	public:

		// EventProcessor<Event> = EffectMapper<T>
		// 因為template不能做多型，所以多切一層interface出來
		/// <summary>
		/// 移動到指定位置上
		///	</summary>
		virtual int Shift(Map* bufferMap, Map* lightMap, EventProcessor<Event>* em) = 0;


	};


	/// <summary>
	/// 移動特效的位置
	///	</summary>
	template<typename T>
	class MapShiftAlgorithm: public MapShiftAlgorithmInterface
	{

	public:

		MapShiftAlgorithm(int sX) {
			startX = sX;
		}

		/// <summary>
		/// 移動到指定位置上
		///	</summary>
		virtual int Shift(Map* bufferMap, Map* lightMap, EventProcessor<Event>* em) {
			if (em->CanCast<EffectMapper<T>>()) {
				return ImplementShift(bufferMap, lightMap, em->Cast<EffectMapper<T>>());
			}

			// TODO: throw error or not?
			return -1;
		}

	protected:

		/// <summary>
		/// 移動到指定位置上
		///	</summary>
		virtual int ImplementShift(Map* bufferMap, Map* lightMap, EffectMapper<T>* em) {

			MTO_FLOAT x = em->GetX() - startX;
			MTO_FLOAT y = em->GetY();

			LOG(LogLevel::Finest) << "MapShiftAlgorithm::ImplementShift(Map*,Map*,EffectMapper<T>*) : Shifting effect [" << em->GetStartTime() << "] to position [" << x << "][" << y << "].";

			MTO_FLOAT width = lightMap->GetWidth();
			MTO_FLOAT height = lightMap->GetHeight();

			// 要確保位移以後，原本再buffer map正中央的洞化會移動到(x,y)的位置，
			// 所以把buffer map從(width-x,height-y)的地方開始切下來，這樣正中央就會跑到(x,y)
			int offsetX = width - x;
			int offsetY = height - y;
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++) {
					// 超出map範圍就跳過
					//if (i - x < 0 || j - y < 0)
					//	continue;

					lightMap->Add(
						i,
						j,
						bufferMap->Get(i + offsetX, j + offsetY)
					);
				}
			}
			return 0;
		}

		int startX;

	};
	

}}}}}








#endif