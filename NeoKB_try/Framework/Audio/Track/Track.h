#ifndef TRACK_H
#define TRACK_H



#include <vector>
#include <thread>
#include <functional>
#include "../AdjustableAudioComponent.h"
#include "../../Timing/AdjustableClock.h"
#include <mutex>


using namespace std;
using namespace Framework::Timing;

namespace Framework {
namespace Audio {
namespace Tracks {

	/// <summary>
	/// 裡面有work queue，每次update就把工作做掉
	/// </summary>
	class Track : public AdjustableAudioComponent, public AdjustableClock {


	public:
		
		// AdjustableAudioComponent
		virtual int Update();

		// Clock
		virtual double GetCurrentTime();
		virtual int SetRate(double r);
		virtual double GetRate();
		virtual int SetIsRunning(bool value);
		virtual bool GetIsRunning();

		// AdjustableClock
		virtual int Start();
		virtual int Stop();
		virtual int Reset();
		virtual int ResetSpeedAdjustments();


	protected:

		Track();

		double length;
		bool isPlayed = false;
		bool isRunning = false;

		double currentTime;
		double rate = 1.0;

		mutable mutex currentTimeMutex;


		/// <summary>
		/// Audio operations will be run on a separate dedicated thread, so we need to schedule any audio API calls using this queue.
		/// </summary>
		// protected ConcurrentQueue<ActionList> PendingActions = new ConcurrentQueue<ActionList>();

	private:


		bool isLoop;

		Bindable<double> tempo;



	};

}}}


#endif