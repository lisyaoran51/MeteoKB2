#include "SampleChannel.h"

using namespace Framework::Audio::Samples;


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

int SampleChannel::Stop()
{
	isPlaying = false;
	return 0;
}

SampleChannel::SampleChannel()
{
	// throw error:�o�Ӧ��ˬd�ΡA���|�u������
	throw runtime_error("SampleChannel::SampleChannel() : ...");
}

SampleChannel::SampleChannel(Sample * s)
{
	if (s == nullptr)
		throw invalid_argument("SampleChannel::SampleChannel(): null argument");

	sample = s;

}
