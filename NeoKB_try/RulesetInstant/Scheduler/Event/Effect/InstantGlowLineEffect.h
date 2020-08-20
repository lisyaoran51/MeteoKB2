#ifndef INSTANT_GLOW_LINE_EFFECT_H
#define INSTANT_GLOW_LINE_EFFECT_H

#include <string>
#include <fstream>
#include <vector>
#include "../../../../Games/Scheduler/Event/Effect/Effect.h"


using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;


namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {
	

	class InstantGlowLineEffect: public Effect
	{

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		InstantGlowLineEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		InstantGlowLineEffect(
			int xPos,
			int yPos,
			MTO_FLOAT sTime,
			MTO_FLOAT l
		);

		// 一定要每次都override!!
		virtual string GetTypeName();

	protected:

	};



	

}}}}



#endif