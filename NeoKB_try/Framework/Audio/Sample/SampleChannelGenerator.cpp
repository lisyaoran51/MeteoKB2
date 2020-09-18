#include "SampleChannelGenerator.h"


using namespace Framework::Audio::Samples;


SampleChannelGenerator::SampleChannelGenerator() : RegisterType("SampleChannelGenerator")
{
}

SampleChannelGenerator::SampleChannelGenerator(CompositeResourceStore<char*>* rStore)
{
	resourceStore = rStore;
}

int SampleChannelGenerator::SetResourceStore(CompositeResourceStore<char*>* rStore)
{
	resourceStore = rStore;

	return 0;
}
