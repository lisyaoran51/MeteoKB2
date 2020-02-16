#include "LinearMapPitchShifter.h"


#include <cmath>


using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace std;


int LinearMapPitchShifter::load()
{

	LOG(LogLevel::Info) << "LinearMapPitchShifter::load() : Start loading map pitch shifter.";

	tempPitch = defaultStartPitch;
	tempPitchSpecificPosition = (double)tempPitch;

	MeteorConfigManager* m = GetCache<MeteorConfigManager>("MeteorConfigManager");

	return load(m);
}

int LinearMapPitchShifter::load(MeteorConfigManager * m)
{

	if (!m->Get(MeteorSetting::PitchShiftSpeed, &defaultMovePerFrame))
		defaultMovePerFrame = 1.0f;

	return 0;
}

LinearMapPitchShifter::LinearMapPitchShifter() : RegisterType("LinearMapPitchShifter")
{

	registerLoad(bind((int(LinearMapPitchShifter::*)())&LinearMapPitchShifter::load, this));

}

int LinearMapPitchShifter::SetSeekPitch(Pitch p)
{
	LOG(LogLevel::Debug) << "LinearMapPitchShifter::SetSeekPitch() : shift from " << (int)tempPitch << " to " << (int)p;
	if (tempPitch == p)
		return 0;

	if ((int)p < (int)lowerBound && tempPitch != lowerBound) {
		pitchShiftingTo = lowerBound;
	}
	else if ((int)p > (int)upperBound && tempPitch != upperBound) {
		pitchShiftingTo = upperBound;
	}
	else
		pitchShiftingTo = p;

	isShifting = true;

	return 0;
}

bool LinearMapPitchShifter::GetIsShifting()
{
	return isShifting;
}

int LinearMapPitchShifter::ResetSeekSpeed()
{
	movePerFrame = defaultMovePerFrame;
	return 0;
}

int LinearMapPitchShifter::SetSeekSpeed(double sSpeed)
{
	double framePerSecond = GetClock()->GetFramesPerSecond();
	if (framePerSecond != 0)
		movePerFrame = sSpeed / framePerSecond;

	// clamp
	if (movePerFrame > 12)
		movePerFrame = 12;
	if (movePerFrame == 0)
		movePerFrame = 0.1;

	return 0;
}

int LinearMapPitchShifter::SetMovePerFrame(double mPerFrame)
{
	movePerFrame = fabs(mPerFrame);
	return 0;
}

Pitch LinearMapPitchShifter::GetTempPitch()
{
	return tempPitch;
}

int LinearMapPitchShifter::shift()
{
	if (isShifting) {

		LOG(LogLevel::Depricated) << "LinearMapPitchShifter::shift() : shift from " << (int)tempPitch << " to " << (int)pitchShiftingTo;
		if (tempPitch > pitchShiftingTo) {

			tempPitchSpecificPosition -= movePerFrame;

			/* �p�Gpitch��F�A�N�^��0 */
			if (tempPitch != (Pitch)(int)floor(tempPitchSpecificPosition)) {
				tempPitch = (Pitch)(int)floor(tempPitchSpecificPosition);

				/* �p�G���ʶW�L�F�A�N�զ^�� */
				if (tempPitch <= pitchShiftingTo) {
					tempPitch = pitchShiftingTo;
					tempPitchSpecificPosition = (double)tempPitch;
					isShifting = false;
					LOG(LogLevel::Debug) << "LinearMapPitchShifter::shift() : shift end with pitch " << (int)tempPitch;
				}
				return 0;
			}
		}
		else if (tempPitch < pitchShiftingTo) {

			tempPitchSpecificPosition += movePerFrame;
			/* �p�Gpitch��F�A�N�^��0 */
			if (tempPitch != (Pitch)(int)floor(tempPitchSpecificPosition)) {
				tempPitch = (Pitch)(int)floor(tempPitchSpecificPosition);

				/* �p�G���ʶW�L�F�A�N�զ^�� */
				if (tempPitch >= pitchShiftingTo) {
					tempPitch = pitchShiftingTo;
					tempPitchSpecificPosition = (double)tempPitch;
					isShifting = false;
					LOG(LogLevel::Debug) << "LinearMapPitchShifter::shift() : shift end with pitch " << (int)tempPitch;
				}
				return 0;
			}
		}
		else {
			// TODO: ���ӭnthrow logic_error
			tempPitch = pitchShiftingTo;
			tempPitchSpecificPosition = (double)tempPitch;
			isShifting = false;

		}
	}

	return -1;


}
