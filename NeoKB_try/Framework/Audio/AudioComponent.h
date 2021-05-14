#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H



#include <vector>
#include <thread>
#include "../../Util/DataStructure/ActionList.h"
#include "../Timing/FramedClock.h"


using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Timing;


namespace Framework {
namespace Audio {

	/// <summary>
	/// 裡面有work queue，每次update就把工作做掉
	/// </summary>
	class AudioComponent {

	public:

		virtual ~AudioComponent() = default;

		bool GetIsCompleted();

		virtual int Update();

		int SetClock(FrameBasedClock* c);

		virtual int _DebugPrintComponents(string spaces);

	protected:

		/// <summary>
		/// Audio operations will be run on a separate dedicated thread, so we need to schedule any audio API calls using this queue.
		/// </summary>
		// protected ConcurrentQueue<ActionList> PendingActions = new ConcurrentQueue<ActionList>();

		virtual int Dispose();

		bool isDisposed = false;

		bool isCompleted = false;

		FrameBasedClock* clock = nullptr;

		ActionList<int> pendingActions;

		ActionList<int> permanentActions;

		mutable mutex pendingActionMutex;

		mutable mutex permanentActionMutex;

	private:

	};

}}


#endif