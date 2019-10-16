#ifndef GAME_THREAD_H
#define GAME_THREAD_H



#include <vector>
#include <thread>
#include <functional>
#include "../Timing/ThrottledFramedClock.h"
#include "../Allocation/Hierachal/Scheduler.h"

using namespace std;
using namespace Framework::Timing;
using namespace Framework::Allocation;


namespace Framework {
namespace Threading {

	class GameThread {

	public:

		GameThread(function<int()> oNewFrame, string tName);

		int Start();

		ThrottledFramedClock* GetClock();

		int SetSleepTime(int sTime); // 懶得弄hz，先暫實用這個，之後再刪掉

	protected:

		string threadName;

	private:

		thread* runThread;

		ThrottledFramedClock* clock;

		Scheduler* scheduler;

		double activeHz;

		double inactiveHz;

		bool exitRequested = false;

		function<int()> onNewFrame;

		int runWork();

		int processFrame();

		int sleepTime;

	};

}}


#endif