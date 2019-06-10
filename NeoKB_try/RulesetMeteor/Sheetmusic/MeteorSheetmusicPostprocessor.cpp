#include "MeteorSheetmusicPostProcessor.h"




using namespace Meteor::Config;
using namespace Meteor::Sheetmusics;


MeteorSmPostprocessor::MeteorSmPostprocessor() 
{
}

Sm<Event>* MeteorSmPostprocessor::postprocess(Sm<Event>* s)
{
	return s;
}
