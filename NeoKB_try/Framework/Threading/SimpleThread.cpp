#include "SimpleThread.h"

#include "ThreadMaster.h"


using namespace Framework::Threading;



SimpleThread::SimpleThread() : RegisterType("SimpleThread")
{
}

int SimpleThread::SetSleepTimeInMilliSecond(int sTimeInMilliSecond)
{
	sleepTimeInMilliSecond = sTimeInMilliSecond;
	return 0;
}
