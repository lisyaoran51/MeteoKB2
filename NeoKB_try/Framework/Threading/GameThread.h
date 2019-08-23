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

		GameThread(function<int()> onNewFrame, string threadName);

		int Start();

		ThrottledFramedClock* GetClock();

	protected:


	private:

		thread* thread;

		ThrottledFramedClock* clock;

		Scheduler scheduler;

		double activeHz;

		double inactiveHz;

		bool exitRequested;

		function<int()> onNewFrame;

		int runWork();

		int processFrame();


	};

}}


#endif