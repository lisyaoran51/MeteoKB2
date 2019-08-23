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
	/// ���[thread�M���ĸ귽
	/// TODO: �O��collection manager�A�O��adjust aduio component
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
		/// �b�[�J�o��manager�ɡA�|�Aadd item�A��manager�\��list��
		/// </summary>
		SampleManager* sampleManager;

		TrackManager* trackManager;

		/// <summary>
		/// �W�O�]��thread�A�baudio manager���غc�l��new
		/// TODO: ��o��thread���U��host��
		/// </summary>
		GameThread* audioThread;

	};

}}


#endif