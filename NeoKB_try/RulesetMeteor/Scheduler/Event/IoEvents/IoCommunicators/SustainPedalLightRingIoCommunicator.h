#ifndef SUSTAIN_PEDAL_LIGHT_RING_IO_COMMUNICATOR_H
#define SUSTAIN_PEDAL_LIGHT_RING_IO_COMMUNICATOR_H


#include "../../../../../Games/Scheduler/Event/IoEvents/IoCommunicator/IoCommunicator.h"
#include "../SustainPedalIoEvent.h"
#include "../SustainPedalLightRing.h"
#include "../../../../../Framework/IO/Panel.h"


using namespace Games::Schedulers::Events::IoEvents::IoCommunicators;
using namespace Meteor::Schedulers::Events::IoEvents;
using namespace Framework::IO;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace IoEvents {
namespace IoCommunicators {



	class SustainPedalLightRingIoCommunicator : public IoCommunicator<SustainPedalIoEvent> {

		int load();

	public:

		SustainPedalLightRingIoCommunicator();

	protected:

		Panel* panel = nullptr;

		virtual int implementProcessIO(SustainPedalLightRing* sPedalLightRing);

		

	};




}}}}}



#endif