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

	class FallEffectMapper : public EffectMapper<FallEffect> {

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		FallEffectMapper();

		FallEffectMapper(int w, int h);

	};



}}}}





#endif