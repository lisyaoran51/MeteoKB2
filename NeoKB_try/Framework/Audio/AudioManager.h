#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H



#include <vector>
#include <thread>
#include <functional>

using namespace std;


namespace Framework {
namespace Audio {

	/// <summary>
	/// ���[thread�M���ĸ귽
	/// TODO: �O��collection manager�A�O��adjust aduio component
	/// </summary>
	class AudioManager {

	public:

		AudioManager();

		int Start();

	protected:


	private:

		SampleManager* sampleManager;

		TrackManager* trackManager;

		/// <summary>
		/// �W�O�]��thread�A�baudio manager���غc�l��new
		/// TODO: ��o��thread���U��host��
		/// </summary>
		thread audioThread;

	};

}}


#endif