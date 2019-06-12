#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H



#include <vector>
#include <thread>
#include <functional>

using namespace std;


namespace Framework {
namespace Audio {

	/// <summary>
	/// 內涵thread和音效資源
	/// TODO: 記成collection manager，記成adjust aduio component
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
		/// 獨力跑的thread，在audio manager的建構子裡new
		/// TODO: 把這個thread註冊到host裡
		/// </summary>
		thread audioThread;

	};

}}


#endif