#ifndef MAPPER_H
#define MAPPER_H


#include "../../../Scheduler/Event/EventProcessor.h"
#include "../../../Scheduler/Event/Effect/Effect.h"
#include "Algorithm/MapAlgorithm.h"
#include <stdexcept>


using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::Effects::Algorithms;


namespace Games {
namespace Schedulers {
namespace Events {
namespace Effects {

	class EffectMapperInterface : virtual public EventProcessor<Event>{

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
		virtual int Draw() = 0;
		virtual int Draw(Map* m) = 0;

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

		virtual int Draw() {
			if (mapAlgo == nullptr) {
				LOG(LogLevel::Error) << "EffectMapper::Draw() : no map algorithm.";
			}


			mapAlgo->Draw(lightMap, this);
			return 0;
		}

		virtual int Draw(Map* m) {
			if(mapAlgo)
			mapAlgo->Draw(m, this);
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
		virtual int GetX(){ return dynamic_cast<Effect*>(GetEffect())->GetX(); }
		virtual int GetY(){ return dynamic_cast<Effect*>(GetEffect())->GetY(); }
		virtual MTO_FLOAT GetSpeed(){ return dynamic_cast<Effect*>(GetEffect())->GetSpeed(); }

	protected:

		Map* lightMap;

		MapAlgorithm<T>* mapAlgo = nullptr;

		int width, height;

	};


}}}}




#endif