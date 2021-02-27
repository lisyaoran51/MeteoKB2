#include "RecordTimeController.h"




using namespace Record::Timing;
using namespace std;



int RecordTimeController::load()
{
	LOG(LogLevel::Info) << "RecordTimeController::load() : loaded. ";
	
	return 0;
}

RecordTimeController::RecordTimeController() : RegisterType("RecordTimeController")
{
	registerLoad(bind(static_cast<int(RecordTimeController::*)(void)>(&RecordTimeController::load), this));
}
