#ifndef GAME_THREAD_H
#define GAME_THREAD_H



#include <vector>
#include <thread>
#include <functional>

using namespace std;


namespace Framework {
namespace Threading {

	class GameThread {

	public:

		GameThread(function<int()> onNewFrame, string threadName);

		int Start();

	protected:


	private:

		thread* thread;

		ThrottledFrameClock* clock;

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