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

		/* ����]�A�ɶ����W�[ */
		if (seekTimeLeft > 0) {

			adjustFrameTime = elapsedTime * adjustSpeed;

			seekTimeLeft -= adjustFrameTime;

			/* �]�L�Y���ܴN�k�s */
			if (seekTimeLeft < 0) {
				adjustFrameTime += seekTimeLeft;
				seekTimeLeft = 0;
				// �o��N�i�Jlast adjusting time
			}
		}
		/* ���^�]�A�ɶ����U�� */
		else if (seekTimeLeft < 0) {

			adjustFrameTime = -elapsedTime * adjustSpeed;

			seekTimeLeft -= adjustFrameTime;

			/* �]�L�Y���ܴN�k�s */
			if (seekTimeLeft > 0) {
				adjustFrameTime += seekTimeLeft;
				seekTimeLeft = 0;
				// �o��N�i�Jlast adjusting time
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
				// �o��|���@�I�I�~�t�A���O�S����k�ץ��^�ӡA���D��time controller�h�վ��e�ɶ�
				freezeTimeLeft = 0;
				// �o��N�i�Jlast freezing time
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

	/* �P�ɥu��freeze�@���ɶ��A�����|�[�A�n�B�~freeze�N�n���o��������Afreeze�@�� */
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
