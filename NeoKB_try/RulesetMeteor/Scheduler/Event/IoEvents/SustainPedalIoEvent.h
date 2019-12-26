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

		int SetTargetTime(MTO_FLOAT tStartTime, MTO_FLOAT tLifeTime, MTO_FLOAT pDownLifeTime, MTO_FLOAT rLifeTime);

		MTO_FLOAT GetTargetStartTime();
		MTO_FLOAT GetTargetLifeTime();
		MTO_FLOAT GetPedalDownLifeTime();
		MTO_FLOAT GetRingLifeTime();


	protected:

		/// <summary>
		/// ��O�ؼ��I�G�_���ɶ��A�N�O����̤W������
		/// </summary>
		MTO_FLOAT targetStartTime = 0;

		/// <summary>
		/// ��O�ؼ��I�|�G�h�[
		/// </summary>
		MTO_FLOAT targetLifeTime = 0;

		/// <summary>
		/// ��O�ݭn��h�[
		/// </summary>
		MTO_FLOAT pedalDownLifeTime = 0;

		/// <summary>
		/// ��O�����`�����ɶ����סC�p�G��O�w�g�����A���OringLifeTime�٨S��A����N�u�|�G��pedalDownLifeTime / ringLifeTime������
		/// </summary>
		MTO_FLOAT ringLifeTime = 0;




	};


}}}}






#endif