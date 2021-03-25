#ifndef INSTANT_DYNAMIC_EVENT_GENERATOR_H
#define INSTANT_DYNAMIC_EVENT_GENERATOR_H



#include "../../../Games/Scheduler/Event/DynamicEventGenerator.h"
#include "../../Input/InstantInputManager.h"
#include "../../../Games/Output/Bluetooths/MeteoBluetoothMessage.h"


using namespace Games::Schedulers::Events;
using namespace Instant::Input;
using namespace Games::Output::Bluetooths;


namespace Instant {
namespace Schedulers{
namespace Events{

	class InstantDynamicEventGenerator : public DynamicEventGenerator, public MessageHandler<MeteoBluetoothMessage> {
	
	public:

		InstantDynamicEventGenerator(Playfield* p);
		
	protected:

		virtual int onMessage(MeteoBluetoothMessage* message);

	};

}}}





#endif