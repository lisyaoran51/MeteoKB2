#ifndef EXPLODE_MAP_ALGORITHM_H
#define EXPLODE_MAP_ALGORITHM_H

#include "../../../../../Base/Graphic/Map/Map.h"
#include "../../../../../Base/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../../../Base/Scheduler/Event/Event.h"
#include "../ExplodeEffectMapper.h"
#include "../ExplodeEffect.h"
#include "../../../../../Base/Scheduler/Event/Effect/Algorithm/MapGenerateAlgorithm.h"
#include "../../../../../Base/Scheduler/Event/Effect/Effect.h"
#include "../../../../../Base/Scheduler/Event/Effect/EffectMapper.h"


using namespace Base::Graphic::Maps;
using namespace Base::Schedulers::Events::Effects::Algorithms;
using namespace Base::Schedulers::Events;
using namespace Base::Schedulers::Events::Effects;
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