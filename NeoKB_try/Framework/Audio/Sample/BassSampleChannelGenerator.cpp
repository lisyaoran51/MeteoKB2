#include "BassSampleChannelGenerator.h"

#include "../../../Instruments/Audio/SimpleSoundBinding.h"
#include "../../../Instruments/Audio/RepeatSoundBinding.h"
#include "../../../Instruments/Audio/TwoStageSoundBinding.h"
#include "../../../Instruments/Pitch.h"
#include "BassSample.h"
#include "BassSampleChannel.h"
#include "DualPlaybackBassSampleChannel.h"
#include "RepeatDualPlaybackBassSampleChannel.h"
#include "TwoStageDualPlaybackBassSampleChannel.h"


using namespace Framework::Audio::Samples;
using namespace Instruments;



BassSampleChannelGenerator::BassSampleChannelGenerator(): SampleChannelGenerator(), RegisterType("BassSampleChannelGenerator")
{
}

BassSampleChannelGenerator::BassSampleChannelGenerator(CompositeResourceStore<char*>* rStore) : SampleChannelGenerator(rStore), RegisterType("BassSampleChannelGenerator")
{
}

SampleChannel * BassSampleChannelGenerator::GenerateSampleChannel(SoundBinding * soundBinding)
{
	LOG(LogLevel::Debug) << "SampleManager::GetSampleChannel() : start generating sample channel [" << soundBinding->GetFileName() << "].";

	SampleChannel* sampleChannel = nullptr;

	/* 從頭播到尾的音色 */
	if (dynamic_cast<SimpleSoundBinding<Pitch>*>(soundBinding)) {
		string path = resourceStore->GetFilePath(soundBinding->GetFileName());

		LOG(LogLevel::Debug) << "SampleManager::GetSampleChannel() : get path [" << path << "].";

		if (path != "") {

			Sample* sample = new BassSample((char*)path.c_str());
			sampleChannel = new DualPlaybackBassSampleChannel(sample);

			LOG(LogLevel::Debug) << "SampleManager::GetSampleChannel() : simple sample file path found [" << soundBinding->GetFileName() << "].";

		}
		else {
			throw runtime_error("SampleManager::GetSampleChannel(): simple sample file not found : "s + soundBinding->GetFileName());
		}
	}
	/* 不停重複的音色 */
	else if(dynamic_cast<RepeatSoundBinding<Pitch>*>(soundBinding)) {
		string path = resourceStore->GetFilePath(soundBinding->GetFileName());

		RepeatSoundBinding<Pitch>* repeatSoundBinding = dynamic_cast<RepeatSoundBinding<Pitch>*>(soundBinding);

		if (path != "") {

			Sample* sample = new BassSample((char*)path.c_str());
			// TODO: 還沒寫好，寫好再來改
			//sampleChannel = new RepeatDualPlaybackBassSampleChannel(sample, repeatSoundBinding->tailLength);

			LOG(LogLevel::Debug) << "SampleManager::GetSampleChannel() : repeat sample file path found [" << soundBinding->GetFileName() << "].";

		}
		else {
			throw runtime_error("SampleManager::GetSampleChannel(): repeat sample file not found : "s + soundBinding->GetFileName());
		}
	}
	/* AB段的音色 */
	else if (dynamic_cast<TwoStageSoundBinding<Pitch>*>(soundBinding)) {
		string path = resourceStore->GetFilePath(soundBinding->GetFileName());

		string pathA = resourceStore->GetFilePath(dynamic_cast<TwoStageSoundBinding<Pitch>*>(soundBinding)->GetStageAFileName());
		string pathB = resourceStore->GetFilePath(dynamic_cast<TwoStageSoundBinding<Pitch>*>(soundBinding)->GetStageBFileName());

		TwoStageSoundBinding<Pitch>* twoStageSoundBinding = dynamic_cast<TwoStageSoundBinding<Pitch>*>(soundBinding);

		if (path != "") {

			Sample* sampleA = new BassSample((char*)pathA.c_str());
			Sample* sampleB = new BassSample((char*)pathB.c_str());
			// TODO: 還沒寫好，寫好再來改
			//sampleChannel = new TwoStageDualPlaybackBassSampleChannel(sampleA, sampleB, twoStageSoundBinding->switchPoint, twoStageSoundBinding->tailLength);

			LOG(LogLevel::Debug) << "SampleManager::GetSampleChannel() : two stage sample file path found [" << soundBinding->GetFileName() << "].";

		}
		else {
			throw runtime_error("SampleManager::GetSampleChannel(): two stage sample file not found : "s + soundBinding->GetFileName());
		}
	}


	return sampleChannel;
}
