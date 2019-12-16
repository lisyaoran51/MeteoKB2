#ifndef EXPLODE_EFFECT_MAPPER_H
#define EXPLODE_EFFECT_MAPPER_H

#include "../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "ExplodeEffect.h"
#include "../../../../Games/Scheduler/Event/HitObject.h"


using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events;



namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {

	class ExplodeEffectMapper :public EffectMapper<ExplodeEffect> {

		double explodeStartTime = 0;

		HitObject* hitObject;

		MTO_FLOAT lifeTime = 0.2f;

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		ExplodeEffectMapper() : EffectMapper() {
			throw logic_error("ExplodeEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
		}

		ExplodeEffectMapper(int w, int h, HitObject* hObject);

		//virtual string GetEventTypeName();

		//virtual MTO_FLOAT GetStartTime();

		//virtual double GetCurrentTime();

		//int SetLifeTime(MTO_FLOAT lTime);

		//virtual MTO_FLOAT GetProcessorTimeLeft();

		//virtual MTO_FLOAT GetLifeTime();

	};



}}}}





#endif