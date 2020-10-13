#include "SampleManager.h"

#include "Sample.h"
#include "BassSample.h"
#include "BassSampleChannel.h"
#include "DualPlaybackBassSampleChannel.h"
#include "MultiPlaybackBassSampleChannel.h"
#include "BassSampleChannelGenerator.h"
#include "../../IO/Storage.h"
#include "../../../Instruments/Audio/Format/SimpleSoundBindingSetDecoder.h"
#include "../../IO/Api/ApiAccess.h"


using namespace Framework::Audio::Samples;
using namespace std;
using namespace std::literals::string_literals;
using namespace Instruments::Audio::Format;
using namespace Framework::IO::Api;



SampleManager::SampleManager(CompositeResourceStore<char*>* rStore) : RegisterType("SampleManager")
{
	resourceStore = rStore;
	sampleChannelGenerator = new BassSampleChannelGenerator(rStore);
	soundBindingSets = new vector<SoundBindingSet*>();
}

bool SampleManager::HasSampleChannel(string name)
{
	map<string, SampleChannel*>::iterator it = sampleChannelCache.find(name);
	if (it != sampleChannelCache.end())
		return true;
	else
		return false;
}

bool SampleManager::HasSampleChannel(SoundBinding * soundBinding)
{
	map<string, SampleChannel*>::iterator it = sampleChannelCache.find(soundBinding->GetFileName());
	if (it != sampleChannelCache.end())
		return true;
	else
		return false;

}

SampleChannel * SampleManager::GetSampleChannel(string name)
{
	Sample* sample = nullptr;
	SampleChannel* sampleChannel = nullptr;

	LOG(LogLevel::Fine) << "SampleManager::GetSampleChannel() : getting channel with file name [" << name << "].";

	map<string, SampleChannel*>::iterator it = sampleChannelCache.find(name);
	if (it == sampleChannelCache.end()) {


		LOG(LogLevel::Fine) << "SampleManager::GetSampleChannel() : resourceStore [" << resourceStore << "].";

		string path = resourceStore->GetFilePath(name);

		LOG(LogLevel::Fine) << "SampleManager::GetSampleChannel() : finding file path [" << path << "].";

		if (path != "") {

			sample = new BassSample((char*)path.c_str());
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

SampleChannel * SampleManager::GetSampleChannel(SoundBinding * soundBinding)
{
	LOG(LogLevel::Fine) << "SampleManager::GetSampleChannel() : action [" << soundBinding->action << "] : file name [" << soundBinding->GetSoundBankName() << "].";

	SampleChannel* sampleChannel = nullptr;


	map<string, SampleChannel*>::iterator it = sampleChannelCache.find(soundBinding->GetSoundBankName());
	if (it == sampleChannelCache.end()) {

		sampleChannel = sampleChannelCache[soundBinding->GetSoundBankName()] = sampleChannelGenerator->GenerateSampleChannel(soundBinding);
		AddItem(sampleChannel);

	}
	else {
		sampleChannel = sampleChannelCache[soundBinding->GetSoundBankName()];
	}

	return sampleChannel;
}

SampleChannel * SampleManager::GetMultiPlaybackSampleChannel(string name)
{
	Sample* sample = nullptr;
	SampleChannel* sampleChannel = nullptr;


	map<string, SampleChannel*>::iterator it = sampleChannelCache.find(name);
	if (it == sampleChannelCache.end()) {

		string path = resourceStore->GetFilePath(name);
		if (path != "") {
			sample = new BassSample((char*)path.c_str());
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

int SampleManager::RemoveSampleChannel(string name)
{
	// TODO: 還沒寫
	return 0;
}

int SampleManager::RemoveSampleChannel(SoundBinding * soundBinding)
{
	LOG(LogLevel::Debug) << "SampleManager::RemoveSampleChannel() : file [" << soundBinding->GetFileName() << "].";
	map<string, SampleChannel*>::iterator it = sampleChannelCache.find(soundBinding->GetFileName());
	if (it != sampleChannelCache.end()) {
		LOG(LogLevel::Depricated) << "SampleManager::RemoveSampleChannel() : start removing.";

		SampleChannel* sampleChannel = (*it).second;
		LOG(LogLevel::Depricated) << "SampleManager::RemoveSampleChannel() : deleting item.";
		deleteItem(sampleChannel);

		pendingActions.Add(this, [=]() {

			LOG(LogLevel::Depricated) << "SampleManager::RemoveSampleChannel() : delete channel.";
			delete sampleChannel;
			return 0;
		}, "Lambda_SampleManager::DeleteSampleChannel");

		
		LOG(LogLevel::Depricated) << "SampleManager::RemoveSampleChannel() : erasing sample channel.";
		sampleChannelCache.erase(it);

		LOG(LogLevel::Depricated) << "SampleManager::RemoveSampleChannel() : file [" << soundBinding->GetFileName() << "] removed from sample cache.";

	}
	else {
		LOG(LogLevel::Error) << "SampleManager::RemoveSampleChannel() : file [" << soundBinding->GetFileName() << "] not found in sample cache.";
	}

	return 0;
}

int SampleManager::ClearSampleChannels()
{
	/* 執行前必須暫停整個Sample系統 */

	map<string, SampleChannel*>::iterator it;

	for (it = sampleChannelCache.begin(); it != sampleChannelCache.end(); it++)
	{
		SampleChannel* smapleChannel = (*it).second;
		deleteItem(smapleChannel);
		delete smapleChannel;
	}
	sampleChannelCache.clear();

	return 0;
}


int SampleManager::SetupApiAccess(ApiAccess * aAccess)
{
	

	aAccess->AddOnWriteSoundFilePackageSuccess(dynamic_cast<MtoObject*>(this), [=](string fileName) {

		// TODO: 現在還沒有要寫下載新音色，所以不用作這個
		LOG(LogLevel::Error) << "SampleManager::SetupApiAccess(): No use when no download sample.";
		throw runtime_error("SampleManager::SetupApiAccess(): not implemented. "s);

		string documentName = fileName.substr(0, fileName.find(string("."), 0));

		FileReader fileReader(documentName);

		import(fileReader);

		return 0;
	});

	return 0;
}

int SampleManager::Import(vector<string>* paths)
{
	// TODO : 送出initializing訊息

	for (int i = 0; i < paths->size(); i++) {
		FileReader fileReader(paths->at(i));
		import(fileReader);
	}

	return 0;
}

int SampleManager::ImportFromStable()
{
	LOG(LogLevel::Info) << "SampleManager::ImportFromStable(): Start Import.";
	Storage* stable = GetStableStorage();

	LOG(LogLevel::Debug) << "SampleManager::ImportFromStable(): Find directory Resources/Samples.";
	vector<string>* paths = stable->GetDirectories("Resources/Samples");

	paths->push_back(stable->GetUsableDirectoryPathFor("Resources/Samples"));

	LOG(LogLevel::Info) << "SampleManager::ImportFromStable() : import paths ^^^" << [](vector<string>* p) {

		for (int i = 0; i < p->size(); i++) {
			LOG(LogLevel::Info) << "---------------------------- " << p->at(i);
		}

		return 0;
	}(paths);

	Import(paths);

	delete stable;
	delete paths;

	return 0;
}

vector<SoundBindingSet*>* SampleManager::GetSoundBindingSets()
{
	return soundBindingSets;
}

vector<SoundBindingSet*>* SampleManager::import(FileReader & fileReader)
{
	// osu的寫法是一個beatmap會有一組set，回傳一組set丟進store李，但是我們的設計沒有需要set，一個普就是一個普，一個資料夾裡應該部會有超過一個普
	importToStorage(fileReader);


	return nullptr;
}

SoundBindingSet * SampleManager::importToStorage(FileReader & fileReader)
{
	vector<string>* sbsNames;

	sbsNames = fileReader.WhereEndWith(".sbs");

	if (sbsNames->size() == 0) {
		LOG(LogLevel::Debug) << "SampleManager::importToStorage(FileReader&) : no sound binding set found.";
		return nullptr;
	}

	for (int i = 0; i < sbsNames->size(); i++) {

		LOG(LogLevel::Finer) << "SampleManager::importToStorage(FileReader&) : Getting stream of [" << sbsNames->at(i) << "].";

		fstream* stream = fileReader.GetStream(sbsNames->at(i));

		// 每一個要用的decoder會在程式開始的時候註冊
		LOG(LogLevel::Finer) << "SampleManager::importToStorage(FileReader&) : Getting decoder of [" << sbsNames->at(i) << "].";
		SoundBindingSetDecoder* soundBindingSetDecoder = SoundBindingSetDecoder::GetDecoder(stream);

		LOG(LogLevel::Finer) << "SampleManager::importToStorage(FileReader&) : Decode [" << sbsNames->at(i) << "] ...";
		SoundBindingSet* sbs = soundBindingSetDecoder->Decode(stream);
		
		soundBindingSets->push_back(sbs);

	}

	// 這一行沒有用，以後有需要再改
	return soundBindingSets->at(0);
}
