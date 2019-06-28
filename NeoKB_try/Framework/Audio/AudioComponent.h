#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H



#include <vector>
#include <thread>
#include <functional>
#include "../Allocation/Hierachal/Updatable.h"


using namespace Framework::Allocation::Hierachal;
using namespace std;


namespace Framework {
namespace Audio {

	/// <summary>
	/// 裡面有work queue，每次update就把工作做掉
	/// </summary>
	class AudioComponent : public Updatable {

	public:


	protected:

		virtual int update();

		/// <summary>
		/// Audio operations will be run on a separate dedicated thread, so we need to schedule any audio API calls using this queue.
		/// </summary>
		// protected ConcurrentQueue<Action> PendingActions = new ConcurrentQueue<Action>();

		bool hasCompleted;

	private:

	};

}}


#endif