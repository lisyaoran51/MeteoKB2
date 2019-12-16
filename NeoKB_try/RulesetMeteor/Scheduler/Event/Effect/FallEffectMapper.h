#ifndef FALL_EFFECT_MAPPER_H
#define FALL_EFFECT_MAPPER_H

#include "../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "FallEffect.h"
#include "../../../../Games/Scheduler/Event/HitObject.h"
#include "../../../../Instruments/HasPitch.h"



using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events;
using namespace Instruments;



namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {

	class FallEffectMapper : public EffectMapper<FallEffect>, public HitObject, public HasPitch {

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		FallEffectMapper();

		FallEffectMapper(int w, int h);

		/// <summary>
		/// �ݬO���O�̫�@�ӽd�򤺪�hit object�A�u���̫�@��hit object�ݭnjudgement
		/// �V����0�V�n�A���O����0�N�O�W�L
		/// </summary>
		virtual double TryJudgement();

		virtual Pitch GetPitch();

	protected:

		virtual int CheckForJudgement(bool isUserTriggered, double timeOffset);

	};



}}}}





#endif