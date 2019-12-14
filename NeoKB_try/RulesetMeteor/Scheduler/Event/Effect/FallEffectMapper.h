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
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		FallEffectMapper() : EffectMapper() {
			throw logic_error("FallEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		FallEffectMapper(int w, int h);

	protected:

		virtual int CheckForJudgement(bool isUserTriggered, double timeOffset);


	};



}}}}





#endif