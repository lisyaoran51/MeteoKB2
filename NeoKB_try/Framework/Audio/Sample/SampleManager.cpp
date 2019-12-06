#include "SampleManager.h"

#include "Sample.h"
#include "BassSample.h"
#include "BassSampleChannel.h"


using namespace Framework::Audio::Samples;
using namespace std;
using namespace std::literals::string_literals;



SampleManager::SampleManager(CompositeResourceStore<char*>* rStore)
{
	resourceStore = rStore;

}

SampleChannel * SampleManager::GetSampleChannel(string name)
{
	Sample* sample = nullptr;
	SampleChannel* sampleChannel = nullptr;


	map<string, Sample*>::iterator it = sampleCache.find(name);
	if (it == sampleCache.end()) {

		string path = resourceStore->GetFilePath(name);
		if (path != "") {
			sample = sampleCache[name] = new BassSample((char*)path.c_str());
			sampleChannel = sampleChannelCache[name] = new BassSampleChannel(sample);
			AddItem(sampleChannel);
		}
		else {
			throw runtime_error("SampleManager::GetSampleChannel(): file not found : "s + name);
		}


		LOG(LogLevel::Debug) << "SampleManager::GetSampleChannel() : file path found [" << path << "].";
		
	}
	else {
		sampleChannel = sampleChannelCache[name];
	}


	return sampleChannel;
}
