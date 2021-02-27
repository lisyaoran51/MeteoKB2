#ifndef METEOR_BUTTON_RECORDER_CONTROLLER_H
#define METEOR_BUTTON_RECORDER_CONTROLLER_H


#include "../MeteorButtonRecorderEvent.h"
#include "../../../../../Games/Scheduler/Event/RecorderEvents/RecorderControllers/RecorderController.h"
#include "../../../../Ruleset/Replays/MeteorReplayRecorder.h"


using namespace Games::Schedulers::Events::RecorderEvents::RecorderControllers;
using namespace Meteor::Rulesets::Replays;


namespace Meteor {
namespace Schedulers{
namespace Events{
namespace RecorderEvents{
namespace RecorderControllers{

	class MeteorButtonRecorderController : public RecorderController<MeteorButtonRecorderEvent> {

		int load();

	public:

		MeteorButtonRecorderController();

	protected:

		MeteorReplayRecorder* meteorReplayRecorder = nullptr;

		virtual int implementAddReplay(EventProcessor<Event>* e);

	};

}}}}}

#endif