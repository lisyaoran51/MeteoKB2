#ifndef FALL_MAP_ALGORITHM_H
#define FALL_MAP_ALGORITHM_H

#include "../../../../../Framework/Graphic/Map/Map.h"
#include "../../../../../Base/Scheduler/Event/Effect/Algorithm/MapGenerateAlgorithm.h"
#include "../../../../../Base/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../../../Base/Scheduler/Event/Effect/Effect.h"
#include "../../../../../Base/Scheduler/Event/Effect/EffectMapper.h"
#include "../FallEffect.h"
#include "../../../../Config/MeteorConfigManager.h"



using namespace Framework::Graphic::Maps;
using namespace Base::Schedulers::Events::Effects::Algorithms;
using namespace Base::Schedulers::Events;
using namespace Base::Schedulers::Events::Effects;
using namespace Meteor::Schedulers::Events::Effects;
using namespace Meteor::Config;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class FallMapAlgorithm: public MapAlgorithm<FallEffect>
	{
		int load();

		int load(MeteorConfigManager * m);

	public:

		/// <summary>
		/// �L�\�Ϊ��غc�l�A�n�f�tlazy construct
		///	</summary>
		FallMapAlgorithm();

	protected:

		int fallLength;

	};

	class FallMapGenerateAlgorithm : public MapGenerateAlgorithm<FallEffect> {

	public:

		FallMapGenerateAlgorithm(MeteorConfigManager* m);

	protected:

		int fallLength;
		MTO_FLOAT fallBrightness;

		/// <summary>
		/// ��evnet�����A�ন�Ϯ�
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<FallEffect>* em);
	};
	

}}}}}








#endif