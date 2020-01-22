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
	/// ���[thread�M���ĸ귽
	/// TODO: �O��collection manager�A�O��adjust aduio component
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
		/// �b�[�J�o��manager�ɡA�|�Aadd item�A��manager�\��list��
		/// </summary>
		SampleManager* sampleManager = nullptr;

		TrackManager* trackManager = nullptr;

		/// <summary>
		/// �W�O�]��thread�A�baudio manager���غc�l��new
		/// TODO: ��o��thread���U��host��
		/// </summary>
		GameThread* audioThread;

	};

}}


#endif