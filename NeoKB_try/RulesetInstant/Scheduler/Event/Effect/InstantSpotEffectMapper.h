#ifndef INSTANT_SPOT_EFFECT_MAPPER_H
#define INSTANT_SPOT_EFFECT_MAPPER_H

#include "../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "InstantSpotEffect.h"


using namespace Games::Schedulers::Events::Effects;


namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {

	class InstantSpotEffectMapper :public EffectMapper<InstantSpotEffect> {

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		InstantSpotEffectMapper() : EffectMapper() {
			throw logic_error("InstantSpotEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		InstantSpotEffectMapper(int w, int h);

	};



}}}}





#endif