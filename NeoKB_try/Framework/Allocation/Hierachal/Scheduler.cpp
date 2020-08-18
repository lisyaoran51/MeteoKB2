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

	LOG(LogLevel::Finer) << "Scheduler::Update() : current time is [" << localCurrentTime << "], timed task [" << timedTasks.size() << "], task queue[" << taskQueue.size() << "].";


	for (int i = 0; i < timedTasks.size(); i++) {

		if (timedTasks[i]->GetExecutionTime() < localCurrentTime) {
			

			if (timedTasks[i]->GetIsCancelled()) {
				delete timedTasks[i];
				timedTasks.erase(timedTasks.begin() + i);
				i--;
				continue;
			}

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
	tasksToSchedule.push_back(task);

	return 0;
}

int Scheduler::AddDelayedTask(function<int(void)> task, double timeUntilRun, bool repeat)
{
	ScheduledTask* scheduledTask = nullptr;

	
	if (repeat)
		scheduledTask = new ScheduledTask(task, clock->GetCurrentTime() + timeUntilRun, timeUntilRun);
	else
		scheduledTask = new ScheduledTask(task, clock->GetCurrentTime() + timeUntilRun);
	

	AddScheduledTask(scheduledTask);

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
	perUpdateTasks.clear();

	isCancelled = true;
	return 0;
}

ScheduledTask::ScheduledTask(function<int(void)> t, double eTime, double rInterval)
{
	task = t;
	executionTime = eTime;
	// 如果repeatInterval是-1代表他不repeat，預設值是-1
	repeatInterval = rInterval;
	isCancelled = false;
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
