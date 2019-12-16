#ifndef FALL_EFFECT_H
#define FALL_EFFECT_H

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
	

	class FallEffect: public Effect, public HasHitWindow, public HasPitch
	{

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		FallEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		FallEffect(
			Pitch p,
			int yPos,
			MTO_FLOAT sTime,
			MTO_FLOAT l,
			MTO_FLOAT sp
		);

		// 一定要每次都override!!
		virtual string GetTypeName();

		Pitch GetPitch();


	protected:
	};



	

}}}}



#endif