#include "InstantSheetmusicPostProcessor.h"



using namespace Instant::Config;
using namespace Instant::Sheetmusics;


InstantSmPostprocessor::InstantSmPostprocessor()
{
}

Sm<Event>* InstantSmPostprocessor::postprocess(Sm<Event>* s)
{
	sort(s->GetEvents()->begin(), s->GetEvents()->end(),
		[](Event* const& a, Event* const& b) { return a->GetStartTime() < b->GetStartTime(); });



	return SmPostprocessor::postprocess(s);
}
