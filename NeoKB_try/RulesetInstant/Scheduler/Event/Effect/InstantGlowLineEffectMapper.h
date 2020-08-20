#ifndef INSTANT_GLOW_LINE_EFFECT_MAPPER_H
#define INSTANT_GLOW_LINE_EFFECT_MAPPER_H

#include "../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "InstantGlowLineEffect.h"


using namespace Games::Schedulers::Events::Effects;


namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {

	class InstantGlowLineEffectMapper :public EffectMapper<InstantGlowLineEffect> {

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		InstantGlowLineEffectMapper() : EffectMapper() {
			throw logic_error("InstantGlowLineEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		InstantGlowLineEffectMapper(int w, int h);

	};



}}}}





#endif