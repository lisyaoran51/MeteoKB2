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
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		InstantGlowLineEffectMapper() : EffectMapper() {
			throw logic_error("InstantGlowLineEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		InstantGlowLineEffectMapper(int w, int h);

	};



}}}}





#endif