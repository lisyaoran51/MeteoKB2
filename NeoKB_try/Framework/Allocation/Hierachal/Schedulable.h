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
		/// �o�ӬOlazy�n�۪��A�ݭn�A��
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