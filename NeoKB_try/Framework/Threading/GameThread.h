#ifndef GAME_THREAD_H
#define GAME_THREAD_H



#include <vector>
#include <thread>
#include <functional>
#include "../Timing/ThrottledFramedClock.h"
#include "../Allocation/Hierachal/Scheduler.h"
#include <sys/types.h>


using namespace std;
using namespace Framework::Timing;
using namespace Framework::Allocation;


/* 
 * ®³pidªº¤èªk
 * http://man7.org/linux/man-pages/man2/syscall.2.html
 * https://stackoverflow.com/questions/15708983/how-can-you-get-the-linux-thread-id-of-a-stdthread
 */


namespace Framework {
namespace Threading {

	class GameThread {

		int statisticFrameRate = 0;

		int currentSecond = 0;

		pid_t threadId = 0;

	public:

		GameThread(function<int()> oNewFrame, string tName);

		string GetName();

		int Start();

		ThrottledFramedClock* GetClock();

		int SetMaxUpdateHz(double mUpdateHz);

		double GetMaxUpdateHz();

		int SetIsActive(bool value);

		bool GetIsActive();


	protected:

		string threadName;

	private:

		thread* runThread;

		ThrottledFramedClock* clock;

		Scheduler* scheduler;

		bool isActive = true;

		double activeHz;

		double inactiveHz = 10;

		bool exitRequested = false;

		function<int()> onNewFrame;

		int runWork();

		int processFrame();

	};

}}


#endif