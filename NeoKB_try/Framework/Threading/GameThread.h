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

		int SetMaxUpdateHz(double mUpdateHz);

		double GetMaxUpdateHz();

		int SetIsActive(bool value);


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