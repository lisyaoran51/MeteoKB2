#ifndef INSTANT_FALL_EFFECT_H
#define INSTANT_FALL_EFFECT_H

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



namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {
	

	class InstantFallEffect: public Effect
	{

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
		///	</summary>
		InstantFallEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		InstantFallEffect(
			int xPos,
			int yPos,
			MTO_FLOAT sTime,
			MTO_FLOAT l,
			MTO_FLOAT sp
		);

		virtual int ApplyDefaultValues(SmDifficulty* smDifficulty);

		int SetTargetHeight(int tHeight);

		/// <summary>
		/// 必須要先設SetTargetHeight，之後才可以adjust speed，不然會跳錯誤，因為沒有target height，會無法換算開始時間
		///	</summary>
		int AdjustSpeed(float sp);

		// 一定要每次都override!!
		virtual string GetTypeName();


	protected:

		int targetHeight = -1;

		bool isSpeedAdjustable = false;


	};



	

}}}}



#endif