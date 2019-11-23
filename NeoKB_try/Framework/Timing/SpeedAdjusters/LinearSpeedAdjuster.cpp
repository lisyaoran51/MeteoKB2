#include "LinearSpeedAdjuster.h"

#include <stdexcept>

using namespace std;
using namespace Framework::Timing::SpeedAdjusters;


LinearSpeedAdjuster::LinearSpeedAdjuster() : RegisterType("LinearSpeedAdjuster")
{
}

int LinearSpeedAdjuster::ProcessFrame(double elapsedTime)
{
	LOG(LogLevel::Finest) << "LinearSpeedAdjuster::ProcessFrame() : process time.";
	if (seekTimeLeft != 0) {
		isAdjustingTime = true;

		/* ����]�A�ɶ����W�[ */
		if (seekTimeLeft > 0) {

			adjustFrameTime = elapsedTime * adjustSpeed;

			seekTimeLeft -= adjustFrameTime;

			/* �]�L�Y���ܴN�k�s */
			if (seekTimeLeft < 0) {
				adjustFrameTime += seekTimeLeft;
				seekTimeLeft = 0;
				isAdjustingTime = false;
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
				isAdjustingTime = false;
			}
		}
	}
	else if (isFreezingTime) {
		freezeTimeLeft -= elapsedTime;
		if (freezeTimeLeft < 0) {
			// �o��|���@�I�I�~�t�A���O�S����k�ץ��^�ӡA���D��time controller�h�վ��e�ɶ�
			freezeTimeLeft = 0;
			isFreezingTime = false;
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
	return isAdjustingTime;
}

bool LinearSpeedAdjuster::GetIsFreezingTime()
{
	return isFreezingTime;
}

int LinearSpeedAdjuster::SetAdjustSpeed(double aSpeed)
{
	if(aSpeed == 0)
		throw invalid_argument("int LinearSpeedAdjuster::SetAdjustSpeed() : error. adjust speed cannot be 0.");
	adjustSpeed = aSpeed;
	return 0;
}
