#include "SampleManager.h"

#include "Sample.h"
#include "BassSample.h"
#include "BassSampleChannel.h"


using namespace Framework::Audio::Samples;
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

		// TODO: 這裡會出錯，沒有家路靜，不過之後再來改
		//ifstream* stream = resourceStore->GetStream(name);
		//if (stream != nullptr) {
			sample = sampleCache[name] = new BassSample((char*)name.c_str());
		//}
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
