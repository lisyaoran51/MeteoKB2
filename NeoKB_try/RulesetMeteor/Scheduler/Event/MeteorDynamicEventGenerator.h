#ifndef METEOR_DYNAMIC_EVENT_GENERATOR_H
#define METEOR_DYNAMIC_EVENT_GENERATOR_H



#include "../../../Games/Scheduler/Event/DynamicEventGenerator.h"
#include "../../Input/MeteorInputManager.h"
#include "../../../Games/Output/Bluetooths/MeteoBluetoothMessage.h"


using namespace Games::Schedulers::Events;
using namespace Meteor::Input;
using namespace Games::Output::Bluetooths;


namespace Meteor {
namespace Schedulers{
namespace Events{

	class MeteorDynamicEventGenerator : public DynamicEventGenerator, public MessageHandler<MeteoBluetoothMessage> {
	
	public:

		MeteorDynamicEventGenerator(Playfield* p);
		
	protected:

		virtual int onMessage(MeteoBluetoothMessage* message);

	};

}}}





#endif