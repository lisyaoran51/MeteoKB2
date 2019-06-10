#ifndef PATTERN_H
#define PATTERN_H


#include <vector>
#include "../../Scheduler/Event/Event.h"


using namespace std;
using namespace Base::Schedulers::Events;


namespace Base {
namespace Sheetmusics {
namespace Patterns {

	/// <summary>
	/// to generate special pattern from original events
	///	</summary>
	class Pattern
	{

	public:

		Pattern();

		Pattern(Event* oEvent);

		int Add(Event* e);

		vector<Event*>* GetEvents();

		int SetOriginalEvent(Event* e);

		Event* GetOriginalEvent();

	protected:

		vector<Event*> events;

		Event* originalEvent = NULL;

	};


}}}


#endif
