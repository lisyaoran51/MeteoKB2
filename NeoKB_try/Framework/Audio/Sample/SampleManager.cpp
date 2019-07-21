#include "SampleManager.h"

#include "Sample.h"


using namespace Framework::Audio::Sample;
using namespace std;

SampleManager::SampleManager(ResourceStore<char*>* rStore)
{
	resourceStore = rStore;

}

SampleChannel * SampleManager::GetSampleChannel(string name)
{
	Sample* sample = nullptr;
	SampleChannel* sampleChannel = nullptr;

	map<string, Sample*>::iterator it = sampleCache.find(name);
	if (it != sampleCache.end()) {

		ifstream* stream = resourceStore->GetStream(name);
		if (stream != nullptr) {
			sample = sampleCache[name] = new BassSample(stream);
		}
	}
	else {
		sample = sampleCache[name];
	}

	if (sample != nullptr) {
		sampleChannel = new BassSampleChannel(sample);
		AddItem(sampleChannel);
	}

	return sampleChannel;
}
