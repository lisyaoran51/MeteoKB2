#include "SampleChannel.h"

using namespace Framework::Audio::Samples;


int SampleChannel::Update()
{
	AudioComponent::Update();
	isPlaying = GetIsPlaying();
	return 0;
}

SampleChannel * SampleChannel::GetSampleChannel(string name)
{
	return nullptr;
}

int SampleChannel::Play()
{
	isStarted = true;
	isPlaying = true;
	return 0;
}

int SampleChannel::Play(double v)
{
	volume->SetValue(v);
	return 0;
}

int SampleChannel::Stop()
{
	isPlaying = false;
	return 0;
}

int SampleChannel::SetFadeOutTime(float fOutTime)
{
	fadeOutTime = fOutTime;
	return 0;
}

/*
SampleChannel::SampleChannel()
{
	// throw error:這個式檢查用，不會真的執行
	throw logic_error("SampleChannel::SampleChannel() : this is for template constraint. unable to launch");
}
*/

SampleChannel::SampleChannel(Sample * s)
{
	if (s == nullptr)
		throw invalid_argument("SampleChannel::SampleChannel(): null argument");

	sample = s;
	initialVolume = 1.0;

}

SampleChannel::~SampleChannel()
{
	LOG(LogLevel::Depricated) << "SampleChannel::~SampleChannel() : deleting sample channel.";
	//delete sample;
	//sample = nullptr;
}
