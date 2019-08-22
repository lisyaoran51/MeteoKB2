#include "BassSample.h"


using namespace Framework::Audio::Sample;



BassSample::BassSample(char * fileName)
{
	sampleID = BASS_SampleLoad(false, fileName, 0, 0, 3, BASS_SAMPLE_OVER_POS);
	isLoaded = true;
}

int BassSample::CreateChannel()
{
	HCHANNEL channel = BASS_SampleGetChannel(sampleID, false);
	return channel;
}
