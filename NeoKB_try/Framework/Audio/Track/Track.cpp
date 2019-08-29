#include "Track.h"


using namespace Framework::Audio::Tracks;


double Track::GetCurrentTime()
{
	return currentTime;
}

int Track::SetRate(double r)
{
	rate = r;
	return 0;
}

double Track::GetRate()
{
	return rate;
}

int Track::SetIsRunning(bool value)
{
	isRunning = value;
	return 0;
}

bool Track::GetIsRunning()
{
	return isRunning;
}

int Track::Start()
{
	if (isDisposed)
		throw new runtime_error("Track::Start(): It's already disposed. Unable to start.");

	return 0;
}

int Track::Stop()
{
	if (isDisposed)
		throw new runtime_error("Track::Stop(): It's already disposed. Unable to stop.");

	return 0;
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

	if (isLoop && isCompleted)
	{
		Reset();
		Start();
	}

	AudioComponent::update();

	return 0;
}
