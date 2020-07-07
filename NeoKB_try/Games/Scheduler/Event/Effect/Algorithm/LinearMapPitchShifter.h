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

		virtual int JumpToPitch(Pitch p);

		virtual int SetSeekPitch(Pitch p);

		virtual bool GetIsShifting();

		virtual int ResetSeekSpeed();

		virtual int SetSeekSpeed(double sSpeed);

		/// <summary>
		/// 因為如果用frame time回推平移速度的話，有的時候剛好那個frame特別快或慢，推出來的速度就會出錯
		/// 所以改成紀錄秒個Frame走了多遠，隨時重新計算速度，才會比較準
		/// </summary>
		virtual int SetFloatSeekSpeed(double sDistanceLeft, double sTimeLeft);

		/// <summary>
		/// 這個應該要少用，frame的速度不固定
		/// </summary>
		int SetMovePerFrame(double mPerFrame);

		virtual Pitch GetTempPitch();

	protected:

		bool isShifting = false;

		Pitch tempPitch;

		Pitch pitchShiftingTo;

		float defaultMovePerFrame = 1.0;

		double movePerFrame = 1.0;

		double floatMovePerFrame = 1.0;

		double tempSeekDistanceLeft = 0;

		double tempSeekTimeLeft = 0;

		double tempPitchSpecificPosition;


		virtual int shift();

		double getFloatMovePerFrame();

		int resetFloatSeekSpeed();
	};


}}}}}



#endif
