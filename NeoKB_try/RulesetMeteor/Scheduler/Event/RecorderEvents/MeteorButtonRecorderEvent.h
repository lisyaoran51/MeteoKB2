#ifndef METEOR_BUTTON_RECORDER_EVENT_H
#define METEOR_BUTTON_RECORDER_EVENT_H


#include "../../../../Games/Scheduler/Event/RecorderEvents/RecorderEvent.h"
#include "../../../../Framework/Input/KeyBindings/InputKey.h"



using namespace Games::Schedulers::Events::RecorderEvents;
using namespace Framework::Input::KeyBindings;



namespace Meteor {
namespace Schedulers{
namespace Events{
namespace RecorderEvents{


	class MeteorButtonRecorderEvent : public RecorderEvent {

	public:

		MeteorButtonRecorderEvent(pair<InputKey, int> c, MTO_FLOAT s, MTO_FLOAT l);

		pair<InputKey, int> GetInput();

		// 一定要每次都override!!
		virtual string GetTypeName();

	protected:

		pair<InputKey, int> command = pair<InputKey, int>();



	};

}}}}







#endif