#include "LinearSpeedAdjuster.h"

#include <stdexcept>

using namespace std;
using namespace Framework::Timing::SpeedAdjusters;


LinearSpeedAdjuster::LinearSpeedAdjuster() : RegisterType("LinearSpeedAdjuster")
{
	adjustSpeed = 4.0;
}

int LinearSpeedAdjuster::ProcessFrame(double elapsedTime)
{
	if (seekTimeLeft != 0) {
		LOG(LogLevel::Finest) << "LinearSpeedAdjuster::ProcessFrame() : seeking time.";
		isAdjustingTime = true;

		/* 往後跑，時間往上加 */
		if (seekTimeLeft > 0) {

			adjustFrameTime = elapsedTime * adjustSpeed;

			seekTimeLeft -= adjustFrameTime;

			/* 跑過頭的話就歸零 */
			if (seekTimeLeft < 0) {
				adjustFrameTime += seekTimeLeft;
				seekTimeLeft = 0;
				// 這邊就進入last adjusting time
			}
		}
		/* 往回跑，時間往下減 */
		else if (seekTimeLeft < 0) {

			adjustFrameTime = -elapsedTime * adjustSpeed;

			seekTimeLeft -= adjustFrameTime;

			/* 跑過頭的話就歸零 */
			if (seekTimeLeft > 0) {
				adjustFrameTime += seekTimeLeft;
				seekTimeLeft = 0;
				// 這邊就進入last adjusting time
			}
		}
	}
	else {
		isAdjustingTime = false;
		adjustFrameTime = 0;

		if (freezeTimeLeft > 0) {
			isFreezingTime = true;
			LOG(LogLevel::Finest) << "LinearSpeedAdjuster::ProcessFrame() : freezing time.";
			freezeTimeLeft -= elapsedTime;
			LOG(LogLevel::Finest) << "LinearSpeedAdjuster::ProcessFrame() : freezing time left [" << freezeTimeLeft << "], elapsed time = [" << elapsedTime << "].";
			if (freezeTimeLeft < 0) {
				// 這邊會有一點點誤差，但是沒有辦法修正回來，除非把time controller去調整當前時間
				freezeTimeLeft = 0;
				// 這邊就進入last freezing time
			}
		}
		else {
			isFreezingTime = false;
			freezeTimeLeft = 0;
		}
	}

	return 0;
}

int LinearSpeedAdjuster::Reset()
{
	seekTimeLeft = 0;
	freezeTimeLeft = 0;
	isAdjustingTime = false;
	isFreezingTime = false;
	return 0;
}

int LinearSpeedAdjuster::SetSeekTime(double sTime)
{
	seekTimeLeft += sTime;
	isAdjustingTime = true;
	return 0;
}

double LinearSpeedAdjuster::GetSeekTime()
{
	return seekTimeLeft;
}

double LinearSpeedAdjuster::GetAdjustFrameTime()
{
	return adjustFrameTime;
}

int LinearSpeedAdjuster::SetFreezeTime(double fTime)
{
	if(fTime < 0)
		throw invalid_argument("int LinearSpeedAdjuster::SetFreezeTime() : error. Freeze time cannot < 0.");

	LOG(LogLevel::Debug) << "LinearSpeedAdjuster::SetFreezeTime() : freezing time for [" << fTime << "].";

	/* 同時只能freeze一次時間，不能疊加，要額外freeze就要等這次結束後再freeze一次 */
	if (!isAdjustingTime && !isFreezingTime) {
		freezeTimeLeft = fTime;
		isFreezingTime = true;
	}

	return 0;
}

double LinearSpeedAdjuster::GetFreezeTimeLeft()
{
	return freezeTimeLeft;
}

bool LinearSpeedAdjuster::GetIsAdjustingTime()
{
	return seekTimeLeft == 0 && adjustFrameTime == 0 ? false : true;
}

bool LinearSpeedAdjuster::GetIsLastAdjustingTime()
{
	return seekTimeLeft == 0 && isAdjustingTime ? true : false;
}

bool LinearSpeedAdjuster::GetIsFreezingTime()
{
	return freezeTimeLeft == 0 ? false : true;
}

bool LinearSpeedAdjuster::GetIsLastFreezingTime()
{
	return freezeTimeLeft == 0 && isFreezingTime ? true : false;
}

int LinearSpeedAdjuster::SetAdjustSpeed(double aSpeed)
{
	if(aSpeed == 0)
		throw invalid_argument("int LinearSpeedAdjuster::SetAdjustSpeed() : error. adjust speed cannot be 0.");
	adjustSpeed = aSpeed;
	return 0;
}
