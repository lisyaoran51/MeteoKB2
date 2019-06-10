#ifndef FALL_EFFECT_MAPPER_H
#define FALL_EFFECT_MAPPER_H

#include "../../../../Base/Scheduler/Event/Effect/EffectMapper.h"
#include "FallEffect.h"


using namespace Base::Schedulers::Events::Effects;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {

	class FallEffectMapper :public EffectMapper<FallEffect> {

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		FallEffectMapper() : EffectMapper() {
			throw logic_error("FallEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		FallEffectMapper(int w, int h);

	};



}}}}





#endif