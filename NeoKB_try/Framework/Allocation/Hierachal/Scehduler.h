#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <vector>
#include "Updatable.h"
#include "../../Timing/FrameBasedClock.h"
#include "../../../Util/DataStructure/Action.h"

using namespace std;
using namespace Framework::Timing;
using namespace Util::DataStructure;

namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// 
	/// </summary>
	class Scheduler {

		FrameBasedClock* clock;

		vector<Action<int(void)>*> taskQueue;

		vector<Action<int(void)>*> taskToDelete;

		vector<ScheduledTask*> timedTasks;

		vector<ScheduledTask*> perUpdateTasks;

		vector<ScheduledTask*> tasksToSchedule;

		vector<ScheduledTask*> tasksToRemove;

	public:

		Scheduler(FrameBasedClock* clock);

		int Update();

		int AddTask(Action<int(void)>* task);

		int AddDelayedTask(Action<int(void)>* task, double timeUntilRun, bool repeat = false);

		int CancelDelayedTasks();

	protected:


	};

	class ScheduledTask {

	};


}}}

#endif