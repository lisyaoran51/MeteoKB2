#ifndef INSTANT_FALL_MAP_ALGORITHM_H
#define INSTANT_FALL_MAP_ALGORITHM_H

#include "../../../../../Framework/Graphic/Map/Map.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapGenerateAlgorithm.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../../../Games/Scheduler/Event/Effect/Effect.h"
#include "../../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "../InstantFallEffect.h"
#include "../../../../Config/InstantConfigManager.h"




using namespace Framework::Graphic::Maps;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::Effects;
using namespace Instant::Schedulers::Events::Effects;
using namespace Instant::Config;

namespace Games {
namespace Scenes{
namespace Play{
	class Playfield;
}}}

namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class InstantFallMapAlgorithm: public MapAlgorithm<InstantFallEffect>
	{
		int load();

		int load(InstantConfigManager * m);

	public:

		/// <summary>
		/// 無功用的建構子，要搭配lazy construct
		///	</summary>
		InstantFallMapAlgorithm();

		int SetPlayfield(Playfield* p);

	protected:

		int fallLength;

		Playfield* playfield = nullptr;

	};

	class InstantFallMapGenerateAlgorithm : public MapGenerateAlgorithm<InstantFallEffect> {

	public:

		InstantFallMapGenerateAlgorithm(InstantConfigManager* m);

	protected:

		int fallLength;
		MTO_FLOAT fallBrightness;

		/// <summary>
		/// 把evnet的狀態轉成圖案
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<InstantFallEffect>* em);
	};
	

}}}}}








#endif