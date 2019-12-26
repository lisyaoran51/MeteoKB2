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

		/// <summary>
		/// �Otarget�I�X�{���ɾ��A���Opedal��U���ɾ�
		/// </summary>
		virtual MTO_FLOAT GetStartTime();

		// �@�w�n�C����override!!
		virtual string GetEventTypeName();

		MTO_FLOAT GetTargetStartTime();
		MTO_FLOAT GetTargetLifeTime();
		MTO_FLOAT GetPedalDownLifeTime();
		MTO_FLOAT GetRingLifeTime();


	};

}}}}


#endif
