#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H



#include <vector>
#include <thread>
#include "../../Util/DataStructure/ActionList.h"


using namespace std;
using namespace Util::DataStructure;


namespace Framework {
namespace Audio {

	/// <summary>
	/// 裡面有work queue，每次update就把工作做掉
	/// </summary>
	class AudioComponent {

	public:

		bool GetIsCompleted();

		virtual int Update();

		virtual int _DebugPrintComponents(string spaces);

	protected:

		/// <summary>
		/// Audio operations will be run on a separate dedicated thread, so we need to schedule any audio API calls using this queue.
		/// </summary>
		// protected ConcurrentQueue<ActionList> PendingActions = new ConcurrentQueue<ActionList>();

		virtual int Dispose();

		bool isDisposed = false;

		bool isCompleted = false;

		ActionList<int> pendingActions;

		mutable mutex pendingActionMutex;

	private:

	};

}}


#endif