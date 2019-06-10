#ifndef FALL_MAP_ALGORITHM_H
#define FALL_MAP_ALGORITHM_H

#include"../../../../../Base/Graphic/Map/Map.h"
#include "../../../../../Base/Scheduler/Event/Effect/Algorithm/MapGenerateAlgorithm.h"
#include "../../../../../Base/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../../../Base/Scheduler/Event/Effect/Effect.h"
#include "../../../../../Base/Scheduler/Event/Effect/EffectMapper.h"
#include "../FallEffect.h"
#include "../../../../Config/MeteorConfigManager.h"



using namespace Base::Graphic::Maps;
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
		/// 無功用的建構子，要搭配lazy construct
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
		/// 把evnet的狀態轉成圖案
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<FallEffect>* em);
	};
	

}}}}}








#endif