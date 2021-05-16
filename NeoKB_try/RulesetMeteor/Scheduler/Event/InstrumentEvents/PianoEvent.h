#ifndef PIANO_EVENT_H
#define PIANO_EVENT_H


#include "../../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEvent.h"
#include "../../../../Framework/Input/KeyBindings/InputKey.h"


using namespace Games::Schedulers::Events::InstrumentEvents;
using namespace Framework::Input::KeyBindings;


namespace Meteor {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{

	class PianoEvent : public InstrumentEvent {

	public:

		PianoEvent(pair<InputKey, int> c, MTO_FLOAT s, MTO_FLOAT l);

		pair<InputKey, int> GetInput();

		// �@�w�n�C����override!!
		virtual string GetTypeName();

		virtual Event* Clone();

	protected:

		pair<InputKey, int> command = pair<InputKey, int>();

	};

}}}}





#endif