#ifndef GLOW_LINE_EFFECT_MAPPER_H
#define GLOW_LINE_EFFECT_MAPPER_H

#include "../../../../Base/Scheduler/Event/Effect/EffectMapper.h"
#include "GlowLineEffect.h"


using namespace Base::Schedulers::Events::Effects;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {

	class GlowLineEffectMapper :public EffectMapper<GlowLineEffect> {

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		GlowLineEffectMapper() : EffectMapper() {
			throw logic_error("GlowLineEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		GlowLineEffectMapper(int w, int h);

	};



}}}}





#endif