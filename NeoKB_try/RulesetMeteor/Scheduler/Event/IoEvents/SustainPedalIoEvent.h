#ifndef SUSTAIN_PEDAL_IO_EVENT_H
#define SUSTAIN_PEDAL_IO_EVENT_H


#include "../../../../Games/Scheduler/Event/IoEvents/PanelEvent.h"


using namespace Games::Schedulers::Events::IoEvents;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace IoEvents {

	class SustainPedalIoEvent : public PanelEvent {

	public:

		SustainPedalIoEvent(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l);

		int SetTargetTime(MTO_FLOAT tStartTime, MTO_FLOAT tLifeTime);

		MTO_FLOAT GetTargetStartTime();
		MTO_FLOAT GetTargetLifeTime();


	protected:

		MTO_FLOAT targetStartTime = 0;
		MTO_FLOAT targetLifeTime = 0;




	};


}}}}






#endif