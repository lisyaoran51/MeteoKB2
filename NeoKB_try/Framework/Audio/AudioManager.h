#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H



#include <vector>
#include <thread>
#include <functional>
#include "AudioCollectionManager.h"
#include "Sample\SampleManager.h"
#include "Track\TrackManager.h"
#include "../IO/Stores/ResourceStore.h"
#include "../Allocation/Hierachal/MtoObject.h"
#include "../Threading/GameThread.h"


using namespace std;
using namespace Framework::Audio::Sample;
using namespace Framework::Audio::Track;
using namespace Framework::IO::Stores;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Threading;




namespace Framework {
namespace Audio {

	/// <summary>
	/// 內涵thread和音效資源
	/// TODO: 記成collection manager，記成adjust aduio component
	/// </summary>
	class AudioManager : public AudioCollectionManager<AdjustableAudioComponent>, public MtoObject {

	public:

		AudioManager(ResourceStore<char*>* trackStore, ResourceStore<char*>* sampleStore);

		SampleManager* GetSampleManager(ResourceStore<char*>* sampleStore = nullptr);

		TrackManager* GetTrackManager(ResourceStore<char*>* trackStore = nullptr);

		GameThread* GetAudioThread();

	protected:


	private:

		/// <summary>
		/// 在加入這些manager時，會再add item，把manager擺到list裡
		/// </summary>
		SampleManager* sampleManager;

		TrackManager* trackManager;

		/// <summary>
		/// 獨力跑的thread，在audio manager的建構子裡new
		/// TODO: 把這個thread註冊到host裡
		/// </summary>
		GameThread* audioThread;

	};

}}


#endif