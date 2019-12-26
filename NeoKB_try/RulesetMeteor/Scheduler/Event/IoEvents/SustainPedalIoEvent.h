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
		/// 踏板目標點亮起的時間，就是光圈最上面那顆
		/// </summary>
		MTO_FLOAT targetStartTime = 0;

		/// <summary>
		/// 踏板目標點會亮多久
		/// </summary>
		MTO_FLOAT targetLifeTime = 0;

		/// <summary>
		/// 踏板需要踏多久
		/// </summary>
		MTO_FLOAT pedalDownLifeTime = 0;

		/// <summary>
		/// 踏板光圈總長的時間長度。如果踏板已經結束，但是ringLifeTime還沒到，光圈就只會亮到pedalDownLifeTime / ringLifeTime的長度
		/// </summary>
		MTO_FLOAT ringLifeTime = 0;




	};


}}}}






#endif