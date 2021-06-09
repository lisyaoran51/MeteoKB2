#include "ThreadMaster.h"

#include "../../Util/Log.h"
#include "../Allocation/Hierachal/ChildAddable.h"
#include "GameThread.h"
#include "SimpleThread.h"

using namespace Framework::Threading;
using namespace Util;
using namespace Framework::Allocation;




int ThreadMaster::AddNewThread(GameThread * thread)
{
	threadProcessing[thread] = false;
	return 0;
}

int ThreadMaster::AddSimpleThread(SimpleThread * t)
{
	simpleThreads[t->GetTypeName()] = t;
	return 0;
}

GameThread * ThreadMaster::GetThread(string threadName)
{

	for (map<GameThread*, bool>::const_iterator it = threadProcessing.begin(); it != threadProcessing.end(); ++it)
	{
		if (it->first->GetName() == threadName)
			return it->first;
	}
	return nullptr;
}

SimpleThread * ThreadMaster::GetSimpleThread(string threadName)
{
	if(simpleThreads.find(threadName) != simpleThreads.end())
		return simpleThreads[threadName];
	else
		return nullptr;
}

int ThreadMaster::SetThreadProcessStatus(GameThread* thread, bool isProcessing)
{

	threadProcessing[thread] = isProcessing;
	return 0;
}

bool ThreadMaster::CheckThreadProcessable()
{
	return !isHolding;
}

bool ThreadMaster::CheckThreadProcessable(string threadName)
{
	return CheckThreadProcessable();
}

int ThreadMaster::HoldAllThreads()
{
	isHolding = true;

	bool allThreadsHolded = true;

	do {
		allThreadsHolded = true;
		for (map<GameThread*, bool>::const_iterator it = threadProcessing.begin(); it != threadProcessing.end(); ++it)
		{
			if (it->second)
				allThreadsHolded = false;
		}
	} 
	while (!allThreadsHolded);



	return 0;
}

int ThreadMaster::ReleaseAllThreads()
{
	isHolding = false;
	return 0;
}

int ThreadMaster::Start()
{


	runThread = new thread(&ThreadMaster::runWork, this);
	runThread->detach();

	return 0;
}

int ThreadMaster::SetFrequency(double hz)
{
	if (hz > 1000)
		hz = 1000;
	runHz = hz;
	return 0;
}

int ThreadMaster::AddObjectToDelete(MtoObject * oToDelete)
{
	objectToDelete.push_back(oToDelete);
	return 0;
}

int ThreadMaster::AddGameStatusThreadAction(int status, MtoObject * callableObject, function<int()> callback, string callbackName)
{
	if (gameStatusThreadActions.find(status) == gameStatusThreadActions.end()) {
		gameStatusThreadActions[status] = new ActionList<int>();
	}

	gameStatusThreadActions[status]->Add(callableObject, callback, callbackName);

	return 0;
}

int ThreadMaster::SwitchGameStatus(int status)
{
	if (gameStatusThreadActions.find(status) != gameStatusThreadActions.end())
		gameStatusThreadActions[status]->Trigger();
	else
		LOG(LogLevel::Error) << "ThreadMaster::SwitchGameStatus : status [" << status << "] not exist.";

	return 0;
}

int ThreadMaster::runWork()
{

	while (!exitRequest) {

		this_thread::sleep_for(chrono::milliseconds(long(1000 / runHz)));

		if (objectToDelete.size() > 0) {
			LOG(LogLevel::Debug) << "ThreadMaster::runWork : try to delete [" << objectToDelete.size() << "] objects.";

			HoldAllThreads();

			for (int i = 0; i < objectToDelete.size(); i++) {
				LOG(LogLevel::Debug) << "ThreadMaster::runWork : deleting object [" << objectToDelete[i]->GetTypeName() << "].";
				
				if (dynamic_cast<ChildAddable*>(objectToDelete[i])) {
					ChildAddable* parent = dynamic_cast<ChildAddable*>(dynamic_cast<ChildAddable*>(objectToDelete[i])->GetParent());
					parent->DeleteChild(dynamic_cast<ChildAddable*>(objectToDelete[i]));

					if(dynamic_cast<ChildAddable*>(objectToDelete[i])->GetChilds()->size() > 0)
						dynamic_cast<ChildAddable*>(objectToDelete[i])->_DebugPrintTree("                       ");

				}


				delete objectToDelete[i];
				objectToDelete.erase(objectToDelete.begin() + i);
				i--;
			}
			ReleaseAllThreads();
		}
	}



	return 0;
}
