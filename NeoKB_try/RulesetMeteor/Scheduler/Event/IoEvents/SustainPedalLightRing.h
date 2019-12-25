#ifndef SUSTAIN_PEDAL_LIGHT_RING_H
#define SUSTAIN_PEDAL_LIGHT_RING_H


#include "../../../../Games/Scheduler/Event/IoEvents/IoEventProcessor.h"
#include "SustainPedalIoEvent.h"


using namespace Games::Schedulers::Events::IoEvents;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace IoEvents {

	class SustainPedalLightRing : public IoEventProcessor<SustainPedalIoEvent> {

	public:

		MTO_FLOAT GetTargetStartTime();
		MTO_FLOAT GetTargetLifeTime();
		MTO_FLOAT GetRingLifeTime();
		MTO_FLOAT GetNextTargetStartTime();

	};

}}}}


#endif
