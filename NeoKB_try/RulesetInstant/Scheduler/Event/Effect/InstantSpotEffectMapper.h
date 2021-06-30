#ifndef INSTANT_SPOT_EFFECT_MAPPER_H
#define INSTANT_SPOT_EFFECT_MAPPER_H

#include "../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "InstantSpotEffect.h"


using namespace Games::Schedulers::Events::Effects;


namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {

	class InstantSpotEffectMapper :public EffectMapper<InstantSpotEffect> {

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		InstantSpotEffectMapper() : EffectMapper() {
			throw logic_error("InstantSpotEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		InstantSpotEffectMapper(int w, int h);

	};



}}}}





#endif