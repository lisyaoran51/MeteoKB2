#include "LinearMapPitchShifter.h"


#include <cmath>


using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace std;


int LinearMapPitchShifter::load()
{

	LOG(LogLevel::Info) << "LinearMapPitchShifter::load() : Start loading map pitch shifter.";

	tempPitch = defaultStartPitch;
	tempPitchSpecificPosition = (double)tempPitch;

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

	pitchShiftingTo = p;
	isShifting = true;

	return 0;
}

bool LinearMapPitchShifter::GetIsShifting()
{
	return isShifting;
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
