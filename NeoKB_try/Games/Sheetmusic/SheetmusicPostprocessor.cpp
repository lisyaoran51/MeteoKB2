#include "SheetmusicPostprocessor.h"


using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events;
using namespace Games::Sheetmusics;



Sm<Event>* SmPostprocessor::postprocess(Sm<Event>* s)
{
	LOG(LogLevel::Fine) << "Sm<Event>* SmPostprocessor::postprocess(Sm<Event>*) : Start sorting ...";

	sort(s->GetEvents()->begin(), s->GetEvents()->end(),
		[](Event* const& a, Event* const& b) { return a->GetStartTime() < b->GetStartTime(); });

	// sort(s->GetEvents()->begin(), s->GetEvents()->end());
	return s;
}


