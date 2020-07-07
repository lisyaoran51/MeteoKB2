#ifndef ERUPT_EFFECT_MAPPER_H
#define ERUPT_EFFECT_MAPPER_H

#include "../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "EruptEffect.h"
#include "../../../../Games/Scheduler/Event/HitObject.h"
#include "../../../../Instruments/HasPitch.h"



using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events;
using namespace Instruments;



namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {

	class EruptEffectMapper : public EffectMapper<EruptEffect> {

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		EruptEffectMapper();

		EruptEffectMapper(int w, int h);

	};



}}}}





#endif