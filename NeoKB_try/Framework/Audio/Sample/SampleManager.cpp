#include "SampleManager.h"

#include "Sample.h"
#include "BassSample.h"
#include "BassSampleChannel.h"
#include "DualPlaybackBassSampleChannel.h"
#include "MultiPlaybackBassSampleChannel.h"


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
			sampleChannel = sampleChannelCache[name] = new DualPlaybackBassSampleChannel(sample);
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

SampleChannel * SampleManager::GetMultiPlaybackSampleChannel(string name)
{
	Sample* sample = nullptr;
	SampleChannel* sampleChannel = nullptr;


	map<string, Sample*>::iterator it = sampleCache.find(name);
	if (it == sampleCache.end()) {

		string path = resourceStore->GetFilePath(name);
		if (path != "") {
			sample = sampleCache[name] = new BassSample((char*)path.c_str());
			//sampleChannel = sampleChannelCache[name] = new MultiPlaybackBassSampleChannel(sample, playbackAmount, 2, OverrideType::MinimunVolume);
			sampleChannel = sampleChannelCache[name] = new MultiPlaybackBassSampleChannel(sample, 1, 2, OverrideType::MinimunVolume);
			AddItem(sampleChannel);
		}
		else {
			throw runtime_error("SampleManager::GetMultiPlaybackSampleChannel(): file not found : "s + name);
		}


		LOG(LogLevel::Fine) << "SampleManager::GetMultiPlaybackSampleChannel() : file path found [" << path << "] for file name [" << name << "].";

	}
	else {
		sampleChannel = sampleChannelCache[name];
	}


	return sampleChannel;
}

int SampleManager::SetPlaybackAmount(int pAmount)
{
	playbackAmount = pAmount;
	return 0;
}
