#ifndef EXPLODE_MAP_ALGORITHM_H
#define EXPLODE_MAP_ALGORITHM_H

#include "../../../../../Framework/Graphic/Map/Map.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../../../Games/Scheduler/Event/Event.h"
#include "../ExplodeEffectMapper.h"
#include "../ExplodeEffect.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapGenerateAlgorithm.h"
#include "../../../../../Games/Scheduler/Event/Effect/Effect.h"
#include "../../../../../Games/Scheduler/Event/Effect/EffectMapper.h"


using namespace Framework::Graphic::Maps;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::Effects;
using namespace Meteor::Schedulers::Events::Effects;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class ExplodeMapAlgorithm: public MapAlgorithm<ExplodeEffect>
	{

		int load();

	public:

		/// <summary>
		/// �L�\�Ϊ��غc�l�A�n�f�tlazy construct
		///	</summary>
		ExplodeMapAlgorithm();

	};


	class ExplodeMapGenerateAlgorithm : public MapGenerateAlgorithm<ExplodeEffect> {
		

	public:

	protected:
		/// <summary>
		/// ��evnet�����A�ন�Ϯ�
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<ExplodeEffect>* em);
	};
	

}}}}}








#endif