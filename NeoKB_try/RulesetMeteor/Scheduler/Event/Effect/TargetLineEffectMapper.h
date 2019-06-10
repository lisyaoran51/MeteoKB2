#ifndef TARGETLINE_EFFECT_MAPPER_H
#define TARGETLINE_EFFECT_MAPPER_H

#include "../../../../Base/Scheduler/Event/Effect/EffectMapper.h"
#include "TargetLineEffect.h"


using namespace Base::Schedulers::Events::Effects;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {

	class TargetLineEffectMapper :public EffectMapper<TargetLineEffect> {

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		TargetLineEffectMapper() : EffectMapper() {
			throw logic_error("FallEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		TargetLineEffectMapper(int w, int h);

	};



}}}}





#endif