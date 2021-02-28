#ifndef METEOR_EFFECT_H
#define METEOR_EFFECT_H

#include <string>
#include <fstream>
#include <vector>
#include "../../../../Games/Scheduler/Event/Effect/Effect.h"
#include "../../../../Instruments/Pitch.h"
#include "../../../../Instruments/HasPitch.h"
#include "../../../../Games/Ruleset/Judgements/HasHitWindow.h"


using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;
using namespace Games::Rulesets::Judgements;
using namespace Instruments;



namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
	
	enum class MeteorEffectShiftType {
		None, 
		RaisedOctave,
		LoweredOctave
	};

	class MeteorEffect: public Effect
	{

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		MeteorEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		MeteorEffect(
			int xPos,
			int yPos,
			MTO_FLOAT sTime,
			MTO_FLOAT l
		);

		int SetMeteorEffectShiftType(MeteorEffectShiftType mEffectShiftType);

	protected:

		MeteorEffectShiftType meteorEffectShiftType = MeteorEffectShiftType::None;

	};



	

}}}}



#endif