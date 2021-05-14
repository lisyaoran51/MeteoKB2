#ifndef SCHEDULABLE_H
#define SCHEDULABLE_H


#include <vector>
#include "Updatable.h"
#include "../../Timing/FrameBasedClock.h"
#include "Scheduler.h"

using namespace std;
using namespace Framework::Timing;

namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// 
	/// </summary>
	class Schedulable : public Updatable {

		/// <summary>
		/// 這個是lazy聲誠的，需要再建
		/// </summary>
		Scheduler* scheduler = nullptr;

	public:

		Schedulable();
		virtual ~Schedulable();

		Scheduler* GetScheduler();

		virtual bool UpdateSubTree();


	protected:


	};

}}}

#endif