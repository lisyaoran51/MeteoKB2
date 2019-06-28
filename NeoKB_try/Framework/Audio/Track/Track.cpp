#include "Track.h"


using namespace Framework::Audio::Track;


double Track::GetCurrentTime()
{
	return 0.0;
}

int Track::SetRate(double r)
{
	return 0;
}

double Track::GetRate()
{
	return 0.0;
}

int Track::SetIsRunning(bool value)
{
	return 0;
}

bool Track::GetIsRunning()
{
	return false;
}

int Track::Reset()
{
	ResetSpeedAdjustments();

	Stop();
	Seek(0);
	return 0;
}

int Track::ResetSpeedAdjustments()
{
	return 0;
}

int Track::update()
{

	if (isLoop && hasCompleted)
	{
		Reset();
		Start();
	}

	return 0;
}
