#ifndef INSTANT_DYNAMIC_EVENT_GENERATOR_H
#define INSTANT_DYNAMIC_EVENT_GENERATOR_H



#include "../../../Games/Scheduler/Event/DynamicEventGenerator.h"
#include "../../Input/InstantInputManager.h"
#include "../../../Games/Input/Commands/MeteoBluetoothCommand.h"


using namespace Games::Schedulers::Events;
using namespace Instant::Input;
using namespace Games::Input::Commands;


namespace Instant {
namespace Schedulers{
namespace Events{

	class InstantDynamicEventGenerator : public DynamicEventGenerator, public CommandHandler<MeteoBluetoothCommand> {
	
	public:

		InstantDynamicEventGenerator(Playfield* p);
		
		virtual int OnCommand(MeteoBluetoothCommand* command);

	};

}}}





#endif