#ifndef MAPPER_H
#define MAPPER_H


#include "../../../Scheduler/Event/EventProcessor.h"
#include "../../../Scheduler/Event/Effect/Effect.h"
#include "Algorithm/MapAlgorithm.h"
#include <stdexcept>


using namespace Base::Schedulers::Events;
using namespace Base::Schedulers::Events::Effects;
using namespace Base::Schedulers::Events::Effects::Algorithms;


namespace Base {
namespace Schedulers {
namespace Events {
namespace Effects {

	class EffectMapperInterface : public EventProcessor<Event>{

	public:

		EffectMapperInterface(): EventProcessor<Event>()
		{
		}

		/// <summary>
		/// after registering the map to mapper, the mapper can apply effect to map.
		/// </summary>
		virtual int RegisterMap(Map* m) = 0;

		virtual int RegisterMapAlgorithm(MapAlgorithmInterface* ma) = 0;

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;
		virtual int GetX() = 0;
		virtual int GetY() = 0;
		virtual MTO_FLOAT GetSpeed() = 0;


	};




	template<class T>
	class EffectMapper : public EffectMapperInterface, private TConstraint<T, Effect> {

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		EffectMapper() : EffectMapperInterface() {
			throw logic_error("EffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		EffectMapper(int w, int h): EffectMapperInterface(){
			width = w;
			height = h;
		}

		virtual int Elapse(MTO_FLOAT elapsedTime) {

			LOG(LogLevel::Finest) << "EffectMapper::Elapse() : Update [" << GetEventTypeName() << "], [" << GetTimeLeft() << "] sec left.";

			if (!lightMap)
				throw runtime_error("int EffectMapper::Process() : no map registered!");

			if (!mapAlgo)
				throw runtime_error("int EffectMapper::Process() : no map algorithm registered!");

			/* current time從effect開始播放時，從0開始計算，直到current time超過life time時，特效結束 */
			currentTime += elapsedTime;

			mapAlgo->Draw(lightMap, this);

			return 0;
		}

		/// <summary>
		/// after registering the map to mapper, the mapper can apply effect to map.
		/// </summary>
		int RegisterMap(Map* m) {
			lightMap = m;

			return 0;
		}

		virtual int RegisterMapAlgorithm(MapAlgorithmInterface* ma) {
			if (MtoObject::CanCast<MapAlgorithm<T>>(ma)) {
				mapAlgo = MtoObject::Cast<MapAlgorithm<T>>(ma);
				return 0;
			}
			else {
				// TODO: 怎麼噴錯誤?
				return -1;
			}
		}

		T* GetEffect() { return dynamic_cast<T*>(event); }

		virtual int GetWidth() { return width; }
		virtual int GetHeight() { return height; }
		virtual int GetX(){ return GetEffect()->GetX(); }
		virtual int GetY(){ return GetEffect()->GetY(); }
		virtual MTO_FLOAT GetSpeed(){ return GetEffect()->GetSpeed(); }

	protected:

		Map* lightMap;

		MapAlgorithm<T>* mapAlgo;

		int width, height;

	};


}}}}




#endif