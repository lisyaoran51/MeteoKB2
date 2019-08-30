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
		virtual int Draw() = 0;
		virtual int Draw(Map* m) = 0;

	};




	template<class T>
	class EffectMapper : public EffectMapperInterface, private TConstraint<T, Effect> {

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		EffectMapper() : EffectMapperInterface() {
			throw logic_error("EffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		EffectMapper(int w, int h): EffectMapperInterface(){
			width = w;
			height = h;
		}

		virtual int Draw() {
			mapAlgo->Draw(lightMap, this);
			return 0;
		}

		virtual int Draw(Map* m) {
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
				// TODO: ���Q���~?
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