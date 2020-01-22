#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H



#include <vector>
#include <thread>
#include <functional>
#include "AudioCollectionManager.h"
#include "Sample/SampleManager.h"
#include "Track/TrackManager.h"
#include "../IO/Stores/CompositeResourceStore.h"
#include "../Allocation/Hierachal/MtoObject.h"
#include "../Threading/GameThread.h"


using namespace std;
using namespace Framework::Audio::Samples;
using namespace Framework::Audio::Tracks;
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

		AudioManager(CompositeResourceStore<char*>* trackStore, CompositeResourceStore<char*>* sampleStore);

		SampleManager* GetSampleManager(CompositeResourceStore<char*>* sampleStore = nullptr);

		TrackManager* GetTrackManager(CompositeResourceStore<char*>* trackStore = nullptr);

		GameThread* GetAudioThread();

		Bindable<double>* GetTrackVolume();

		Bindable<double>* GetSampleVolume();


	protected:

		Bindable<double>* trackVolume = new Bindable<double>(0.5);
		Bindable<double>* sampleVolume = new Bindable<double>(1.0);

	private:

		/// <summary>
		/// 在加入這些manager時，會再add item，把manager擺到list裡
		/// </summary>
		SampleManager* sampleManager = nullptr;

		TrackManager* trackManager = nullptr;

		/// <summary>
		/// 獨力跑的thread，在audio manager的建構子裡new
		/// TODO: 把這個thread註冊到host裡
		/// </summary>
		GameThread* audioThread;

	};

}}


#endif