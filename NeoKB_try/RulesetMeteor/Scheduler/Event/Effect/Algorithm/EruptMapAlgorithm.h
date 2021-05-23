#ifndef ERUPT_MAP_ALGORITHM_H
#define ERUPT_MAP_ALGORITHM_H




#include "../../../../../Framework/Graphic/Map/Map.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapGenerateAlgorithm.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../../../Games/Scheduler/Event/Effect/Effect.h"
#include "../../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "../EruptEffect.h"
#include "../../../../Config/MeteorConfigManager.h"



using namespace Framework::Graphic::Maps;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::Effects;
using namespace Meteor::Schedulers::Events::Effects;
using namespace Meteor::Config;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class EruptMapAlgorithm: public MapAlgorithm<EruptEffect>
	{
		int load();

		int load(MeteorConfigManager * m);

	public:

		/// <summary>
		/// 無功用的建構子，要搭配lazy construct
		///	</summary>
		EruptMapAlgorithm();

	protected:

		int fallLength;

	};

	class EruptMapGenerateAlgorithm : public MapGenerateAlgorithm<EruptEffect> {

	public:

		EruptMapGenerateAlgorithm(MeteorConfigManager* m);

	protected:

		int fallLength;
		MTO_FLOAT fallBrightness;
		int whiteKeyTargetHeight;

		/// <summary>
		/// 把evnet的狀態轉成圖案
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<EruptEffect>* em);
	};
	

}}}}}












#endif