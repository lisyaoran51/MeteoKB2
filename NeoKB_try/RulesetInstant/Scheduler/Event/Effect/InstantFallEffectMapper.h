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
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		InstantFallEffectMapper();

		InstantFallEffectMapper(int w, int h);

	};



}}}}





#endif