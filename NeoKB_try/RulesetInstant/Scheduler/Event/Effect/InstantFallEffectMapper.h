#ifndef INSTANT_FALL_EFFECT_MAPPER_H
#define INSTANT_FALL_EFFECT_MAPPER_H

#include "../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "InstantFallEffect.h"
#include "../../../../Games/Scheduler/Event/HitObject.h"
#include "../../../../Instruments/HasPitch.h"



using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events;
using namespace Instruments;



namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {

	class InstantFallEffectMapper : public EffectMapper<InstantFallEffect> {

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		InstantFallEffectMapper();

		InstantFallEffectMapper(int w, int h);

	};



}}}}





#endif