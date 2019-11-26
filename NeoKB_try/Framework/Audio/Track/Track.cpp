#include "Track.h"


using namespace Framework::Audio::Tracks;


Track::Track()
{
	// throw error:這個式檢查用，不會真的執行
	//throw logic_error("Track::Track() : this is for template constraint. unable to launch");
}

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
	LOG(LogLevel::Depricated) << "Track::GetIsRunning() : if the track is running = [" << isRunning << "].";
	return isRunning;
}

int Track::Start()
{
	if (isDisposed)
		throw runtime_error("Track::Start(): It's already disposed. Unable to start.");

	return 0;
}

int Track::Stop()
{
	if (isDisposed)
		throw runtime_error("Track::Stop(): It's already disposed. Unable to stop.");

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

int Track::Update()
{

	if (isLoop && isCompleted)
	{
		Reset();
		Start();
	}

	AudioComponent::Update();

	return 0;
}
