#include "Scheduler.h"


using namespace Framework::Allocation::Hierachal;


Scheduler::Scheduler(FrameBasedClock * c)
{
	clock = c;
}

int Scheduler::Update()
{

	if (isCancelled) {
		Cancel();
		return 0;
	}

	double localCurrentTime = clock->GetCurrentTime();

	for (int i = 0; i < timedTasks.size(); i++) {

		if (timedTasks[i]->GetExecutionTime < localCurrentTime) {
			

			if (timedTasks[i]->GetIsCancelled())
				continue;

			taskQueue.push_back(timedTasks[i]->GetTask());

			if (timedTasks[i]->GetRepeatInterval() > 0) {
				timedTasks[i]->SetExecutionTime(timedTasks[i]->GetExecutionTime() + timedTasks[i]->GetRepeatInterval());
				tasksToSchedule.push_back(timedTasks[i]);
			}
			else {
				delete timedTasks[i];
			}

			timedTasks.erase(timedTasks.begin() + i);
			i--;

		}

	}

	/* 加入新任務 */
	for (int i = 0; i < tasksToSchedule.size(); i++) {
		timedTasks.push_back(tasksToSchedule[i]);
	}
	tasksToSchedule.clear();

	/* 每次都跑 */
	for (int i = 0; i < perUpdateTasks.size(); i++) {
		taskQueue.push_back(perUpdateTasks[i]->GetTask());
	}

	/* 開始執行 */
	for (int i = 0; i < taskQueue.size(); i++) {
		taskQueue[i]();
	}
	taskQueue.clear();


	return 0;
}

int Scheduler::AddTask(function<int(void)> task)
{
	taskQueue.push_back(task);

	return 0;
}

int Scheduler::AddScheduledTask(ScheduledTask * task)
{
	for (int i = 0; i < timedTasks.size(); i++)
		delete timedTasks[i];

	timedTasks.clear();



	return 0;
}

int Scheduler::AddDelayedTask(function<int(void)> task, double timeUntilRun, bool repeat = false)
{
	ScheduledTask* scheduledTask = nullptr;

	if (repeat)
		scheduledTask = new ScheduledTask(task, clock->GetCurrentTime() + timeUntilRun, timeUntilRun);
	else
		scheduledTask = new ScheduledTask(task, clock->GetCurrentTime() + timeUntilRun, -1);

	tasksToSchedule.push_back(scheduledTask);

	return 0;
}

int Scheduler::CancelDelayedTasks()
{
	tasksToSchedule.clear();
	timedTasks.clear();

	return 0;
}

int Scheduler::Cancel()
{
	CancelDelayedTasks();
	taskQueue.clear();
	isCancelled = true;
	return 0;
}

ScheduledTask::ScheduledTask(function<int(void)> t, double eTime, double rInterval)
{
	task = t;
	executionTime = eTime;
	repeatInterval = rInterval;
}

int ScheduledTask::SetExecutionTime(double eTime)
{
	executionTime = eTime;
	return 0;
}

function<int(void)> ScheduledTask::GetTask()
{
	return task;
}

double ScheduledTask::GetExecutionTime()
{
	return executionTime;
}

double ScheduledTask::GetRepeatInterval()
{
	return repeatInterval;
}

bool ScheduledTask::GetIsCancelled()
{
	return isCancelled;
}

int ScheduledTask::Cancel()
{
	isCancelled = true;
	return 0;
}
