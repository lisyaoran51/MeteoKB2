#include "ThreadMaster.h"

#include "../../Util/Log.h"

using namespace Framework::Threading;
using namespace Util;


int ThreadMaster::AddNewThread(string threadName)
{
	threadProcessing[threadName] = false;

	return 0;
}

int ThreadMaster::SetThreadProcessStatus(string threadName, bool isProcessing)
{
	threadProcessing[threadName] = isProcessing;
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
		for (map<string, bool>::const_iterator it = threadProcessing.begin(); it != threadProcessing.end(); ++it)
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

int ThreadMaster::runWork()
{
	this_thread::sleep_for(chrono::milliseconds(long(runHz / 1000)));

	if (objectToDelete.size() > 0) {
		HoldAllThreads();

		for (int i = 0; i < objectToDelete.size(); i++) {
			LOG(LogLevel::Debug) << "ThreadMaster::runWork : deleting object [" << objectToDelete[i]->GetTypeName() << "].";
			delete objectToDelete[i];
			i--;
		}
		ReleaseAllThreads();
	}



	return 0;
}
