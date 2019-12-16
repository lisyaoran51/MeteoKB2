#ifndef EXPLODE_EFFECT_H
#define EXPLODE_EFFECT_H

#include <string>
#include <fstream>
#include <vector>
#include "../../../../Games/Scheduler/Event/Effect/Effect.h"
#include "../../../../Instruments/Pitch.h"


using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;
using namespace Instruments;




namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
	

	class ExplodeEffect: public Effect
	{

		Pitch pitch;

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		ExplodeEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		ExplodeEffect(
			Pitch p,
			int yPos,
			MTO_FLOAT sTime,
			MTO_FLOAT l
		);

		Pitch GetPitch();

		// 一定要每次都override!!
		virtual string GetTypeName();

	protected:
	};



	

}}}}



#endif