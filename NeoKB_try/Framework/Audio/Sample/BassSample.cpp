#include "BassSample.h"


using namespace Framework::Audio::Samples;



BassSample::BassSample(char * fileName)
{
	sampleID = BASS_SampleLoad(false, fileName, 0, 0, 128, BASS_SAMPLE_OVER_POS);
	isLoaded = true;
}

int BassSample::CreateChannel()
{
	HCHANNEL channel = BASS_SampleGetChannel(sampleID, true);
	return channel;
}
