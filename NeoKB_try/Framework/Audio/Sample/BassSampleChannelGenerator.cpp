#include "BassSampleChannelGenerator.h"


using namespace Framework::Audio::Samples;


BassSampleChannelGenerator::BassSampleChannelGenerator(): SampleChannelGenerator(), RegisterType("BassSampleChannelGenerator")
{
}

BassSampleChannelGenerator::BassSampleChannelGenerator(CompositeResourceStore<char*>* rStore) : SampleChannelGenerator(rStore), RegisterType("BassSampleChannelGenerator")
{
}

SampleChannel * BassSampleChannelGenerator::GenerateSampleChannel(SoundBinding * soundBinding)
{






	return nullptr;
}
