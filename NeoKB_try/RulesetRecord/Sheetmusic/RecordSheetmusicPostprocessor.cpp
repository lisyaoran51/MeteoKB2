#include "RecordSheetmusicPostProcessor.h"


using namespace Record::Sheetmusics;


RecordSmPostprocessor::RecordSmPostprocessor()
{
}

Sm<Event>* RecordSmPostprocessor::postprocess(Sm<Event>* s)
{
	sort(s->GetEvents()->begin(), s->GetEvents()->end(),
		[](Event* const& a, Event* const& b) { return a->GetStartTime() < b->GetStartTime(); });



	return SmPostprocessor::postprocess(s);
}
