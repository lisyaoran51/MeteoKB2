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
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		GlowLineEffectMapper() : EffectMapper() {
			throw logic_error("GlowLineEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		GlowLineEffectMapper(int w, int h);

	};



}}}}





#endif