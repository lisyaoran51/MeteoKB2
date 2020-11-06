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

		SustainPedalLightRing();

		bool GetHasNotifyInAdvance();

		/// <summary>
		/// 是target點出現的時機，不是pedal踏下的時機
		/// </summary>
		virtual MTO_FLOAT GetStartTime();


		MTO_FLOAT GetTargetStartTime();
		MTO_FLOAT GetTargetLifeTime();
		MTO_FLOAT GetPedalDownLifeTime();
		MTO_FLOAT GetRingLifeTime();


	};

}}}}


#endif
