#include "BassSample.h"

#include "../../../Util/Log.h"


using namespace Framework::Audio::Samples;
using namespace Util;


BassSample::BassSample(char * fileName)
{
	sampleID = BASS_SampleLoad(false, fileName, 0, 0, 128, BASS_SAMPLE_OVER_POS);
	isLoaded = true;
}

BassSample::~BassSample()
{
	LOG(LogLevel::Depricated) << "BassSample::~BassSample() : delete sample.";
	BASS_SampleFree(sampleID);
}

int BassSample::CreateChannel()
{
	HCHANNEL channel = BASS_SampleGetChannel(sampleID, true);
	return channel;
}
