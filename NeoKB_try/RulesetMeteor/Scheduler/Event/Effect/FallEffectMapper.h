#ifndef FALL_EFFECT_MAPPER_H
#define FALL_EFFECT_MAPPER_H

#include "../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "FallEffect.h"
#include "../../../../Games/Scheduler/Event/HitObject.h"



using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events;



namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {

	class FallEffectMapper : public EffectMapper<FallEffect>, public HitObject {

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		FallEffectMapper() : EffectMapper() {
			throw logic_error("FallEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		FallEffectMapper(int w, int h);

		/// <summary>
		/// 看是不是最後一個範圍內的hit object，只有最後一個hit object需要judgement
		/// 越接近0越好，但是等於0就是超過
		/// </summary>
		virtual double TryJudgement();

		Pitch GetPitch();

	protected:

		virtual int CheckForJudgement(bool isUserTriggered, double timeOffset);

	};



}}}}





#endif