#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <vector>
#include "Updatable.h"
#include "../../Timing/FrameBasedClock.h"
#include "../../../Util/DataStructure/ActionList.h"

using namespace std;
using namespace Framework::Timing;
using namespace Util::DataStructure;

namespace Framework {
namespace Allocation {
namespace Hierachal {

	class ScheduledTask;


	/// <summary>
	/// 
	/// </summary>
	class Scheduler {

		FrameBasedClock* clock;

		vector<function<int(void)>> taskQueue;

		vector<function<int(void)>> taskToDelete;

		vector<ScheduledTask*> timedTasks;

		vector<ScheduledTask*> perUpdateTasks;

		vector<ScheduledTask*> tasksToSchedule;

		vector<ScheduledTask*> tasksToRemove;

		bool isCancelled = false;

	public:

		Scheduler(FrameBasedClock* c);

		int Update();

		/// <summary>
		/// 立刻執行的task，要小心concurrency競爭
		/// </summary>
		int AddTask(function<int(void)> task);

		int AddScheduledTask(ScheduledTask* task);

		int AddDelayedTask(function<int(void)> task, double timeUntilRun, bool repeat = false);

		int CancelDelayedTasks();

		/// <summary>
		/// 這個寒士沒有真的把task刪掉，只把vector清空，所以會memory leak
		/// </summary>
		int Cancel();

	protected:


	};

	class ScheduledTask {

		function<int(void)> task;

		/// <summary>
		/// The earliest ElapsedTime value at which we can be executed.
		/// </summary>
		double executionTime;

		/// <summary>
		/// Time in milliseconds between repeats of this task. -1 means no repeats.
		/// </summary>
		double repeatInterval;

		bool isCancelled;

	public:

		ScheduledTask(function<int(void)> t, double eTime, double rInterval = -1);

		int SetExecutionTime(double eTime);

		function<int(void)> GetTask();
		
		double GetExecutionTime();

		double GetRepeatInterval();

		bool GetIsCancelled();

		int Cancel();
	};


}}}

#endif