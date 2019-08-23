#include "Schedulable.h"


using namespace Framework::Allocation::Hierachal;


Schedulable::Schedulable() : RegisterType("Schedulable")
{
}

Scheduler * Schedulable::GetScheduler()
{
	if (scheduler != nullptr)
		return scheduler;
	else
		return scheduler = new Scheduler(GetClock());
}

bool Schedulable::UpdateSubTree()
{
	Updatable::UpdateSubTree();

	if (scheduler != nullptr)
		scheduler->Update();

	return true;
}
