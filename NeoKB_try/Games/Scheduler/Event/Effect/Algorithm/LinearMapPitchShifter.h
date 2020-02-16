#ifndef LINEAR_MAP_PITCH_SHIFTER_H
#define LINEAR_MAP_PITCH_SHIFTER_H

#include "MapPitchShifter.h"
#include "../../../../../RulesetMeteor/Config/MeteorConfigManager.h"


using namespace Meteor::Config;


namespace Games {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{

	/// <summary>
	/// 用來把map平移到想要的pitch的工具，移動方式是線性移動
	/// </summary>
	class LinearMapPitchShifter : public MapPitchShifter {

		int load();

		int load(MeteorConfigManager* m);

	public:

		LinearMapPitchShifter();

		virtual int SetSeekPitch(Pitch p);

		virtual bool GetIsShifting();

		virtual int ResetSeekSpeed();

		virtual int SetSeekSpeed(double sSpeed);

		int SetMovePerFrame(double mPerFrame);

		virtual Pitch GetTempPitch();

	protected:

		bool isShifting = false;

		Pitch tempPitch;

		Pitch pitchShiftingTo;

		float defaultMovePerFrame = 1.0;

		double movePerFrame = 1.0;

		double tempPitchSpecificPosition;


		virtual int shift();


	};


}}}}}



#endif
