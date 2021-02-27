#ifndef RECORD_DYNAMIC_EVENT_GENERATOR_H
#define RECORD_DYNAMIC_EVENT_GENERATOR_H



#include "../../../Games/Scheduler/Event/DynamicEventGenerator.h"
#include "../../Input/RecordInputManager.h"
#include "../../../Games/Output/Bluetooths/MeteoBluetoothMessage.h"


using namespace Games::Schedulers::Events;
using namespace Record::Input;
using namespace Games::Output::Bluetooths;


namespace Record {
namespace Schedulers{
namespace Events{

	class RecordDynamicEventGenerator : public DynamicEventGenerator, public MessageHandler<MeteoBluetoothMessage> {
	
	public:

		RecordDynamicEventGenerator(Playfield* p);
		
		virtual int OnMessage(MeteoBluetoothMessage* message);

	};

}}}





#endif