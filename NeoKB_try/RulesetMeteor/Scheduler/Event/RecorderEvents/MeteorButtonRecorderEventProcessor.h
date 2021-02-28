#ifndef METEOR_BUTTON_RECORDER_EVENT_PROCESSOR_H
#define METEOR_BUTTON_RECORDER_EVENT_PROCESSOR_H




#include "../../../../Games/Scheduler/Event/RecorderEvents/RecorderEventProcessor.h"
#include "MeteorButtonRecorderEvent.h"
#include <utility>

using namespace Games::Schedulers::Events::RecorderEvents;
using namespace std;


namespace Meteor {
namespace Schedulers{
namespace Events{
namespace RecorderEvents{

	class MeteorButtonRecorderEventProcessor : public RecorderEventProcessor<MeteorButtonRecorderEvent> {

	public:

		pair<InputKey, int> GetInput();


	};

}}}}






#endif