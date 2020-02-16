#include "PlayfieldEventProcessor.h"


using namespace Games::Schedulers::Events::PlayfieldEvents;


int PlayfieldEventProcessorInterface::Process()
{
	return ControlPlayfield();
}
