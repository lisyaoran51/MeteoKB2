#ifndef EXPLODE_EFFECT_MAPPER_H
#define EXPLODE_EFFECT_MAPPER_H

#include "../../../../Base/Scheduler/Event/Effect/EffectMapper.h"
#include "ExplodeEffect.h"


using namespace Base::Schedulers::Events::Effects;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {

	class ExplodeEffectMapper :public EffectMapper<ExplodeEffect> {

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		ExplodeEffectMapper() : EffectMapper() {
			throw logic_error("ExplodeEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		ExplodeEffectMapper(int w, int h);

	};



}}}}





#endif