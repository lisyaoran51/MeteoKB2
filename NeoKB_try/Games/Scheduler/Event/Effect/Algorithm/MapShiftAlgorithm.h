#ifndef MAP_SHIFT_ALGORITHM_H
#define MAP_SHIFT_ALGORITHM_H

#include "../../../../../Framework/Graphic/Map/Map.h"
#include "../../Event.h"
#include "../../EventProcessor.h"

using namespace Framework::Graphic::Maps;

namespace Games {
namespace Schedulers {
namespace Events {
namespace Effects {

	template<typename T>
	class EffectMapper;

}}}}

using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events;

namespace Games {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms {

	class MapShiftAlgorithmInterface {

	public:

		// EventProcessor<Event> = EffectMapper<T>
		// �]��template���వ�h���A�ҥH�h���@�hinterface�X��
		/// <summary>
		/// ���ʨ���w��m�W
		///	</summary>
		virtual int Shift(Map* bufferMap, Map* lightMap, EventProcessor<Event>* em) = 0;


	};


	/// <summary>
	/// ���ʯS�Ī���m
	///	</summary>
	template<typename T>
	class MapShiftAlgorithm : public MapShiftAlgorithmInterface
	{

	public:

		MapShiftAlgorithm(int sX) {
			startX = sX;
		}

		/// <summary>
		/// ���ʨ���w��m�W
		///	</summary>
		virtual int Shift(Map* bufferMap, Map* lightMap, EventProcessor<Event>* em) {
			if (em->CanCast<EffectMapper<T>>()) {
				return ImplementShift(bufferMap, lightMap, em->Cast<EffectMapper<T>>());
			}

			// TODO: throw error or not?
			return -1;
		}


		

		virtual int SetStartX(int x) {
			startX = x;
			return 0;
		}

protected:

	/// <summary>
	/// ���ʨ���w��m�W
	///	</summary>
	virtual int ImplementShift(Map* bufferMap, Map* lightMap, EffectMapper<T>* em) {

		MTO_FLOAT x = em->GetX() - startX;
		MTO_FLOAT y = em->GetY();

		LOG(LogLevel::Finest) << "MapShiftAlgorithm::ImplementShift(Map*,Map*,EffectMapper<T>*) : Shifting effect [" << em->GetStartTime() << "] to position [" << x << "][" << y << "].";

		MTO_FLOAT width = lightMap->GetWidth();
		MTO_FLOAT height = lightMap->GetHeight();

		// �n�T�O�첾�H��A�쥻�Abuffer map���������}�Ʒ|���ʨ�(x,y)����m�A
		// �ҥH��buffer map�q(width-x,height-y)���a��}�l���U�ӡA�o�˥������N�|�]��(x,y)
		int offsetX = width - x;
		int offsetY = height - y;
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				// �W�Xmap�d��N���L
				//if (i - x < 0 || j - y < 0)
				//	continue;
				if (bufferMap->Get(i + offsetX, j + offsetY) == 0)
					continue;

				lightMap->Add(
					i,
					j,
					bufferMap->Get(i + offsetX, j + offsetY)
				);
			}
		}
		return 0;
	}

	int startX = 24;

};
	

}}}}}








#endif