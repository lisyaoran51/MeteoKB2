#ifndef SUSTAIN_PEDAL_LIGHT_RING_IO_COMMUNICATOR_H
#define SUSTAIN_PEDAL_LIGHT_RING_IO_COMMUNICATOR_H


#include "../../../../../Games/Scheduler/Event/IoEvents/IoCommunicator/IoCommunicator.h"
#include "../SustainPedalIoEvent.h"


using namespace Games::Schedulers::Events::IoEvents::IoCommunicators;
using namespace Meteor::Schedulers::Events::IoEvents;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace IoEvents {
namespace IoCommunicators {

	class SustainPedalLightRingIoCommunicator : public IoCommunicator<SustainPedalIoEvent> {

	public:

		SustainPedalLightRingIoCommunicator();


	};




}}}}}



#endif