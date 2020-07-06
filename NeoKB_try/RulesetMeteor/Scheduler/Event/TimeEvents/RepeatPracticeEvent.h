#ifndef REPEAT_PRACTICE_EVENT_H
#define REPEAT_PRACTICE_EVENT_H


#include "../../../../Games/Scheduler/Event/TimeEvents/TimeEvent.h"


using namespace Games::Schedulers::Events::TimeEvents;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace TimeEvents {

	class RepeatPracticeEvent : public TimeEvent {

	public:

		RepeatPracticeEvent(int section, float rLength, MTO_FLOAT s, MTO_FLOAT l);

		// 一定要每次都override!!
		virtual string GetTypeName();

		int GetRepeatSection();

		float GetRewindLength();

	protected:

		/// <summary>
		/// 重複的小節
		/// </summary>
		int repeatSection = -1;

		/// <summary>
		/// 要往前迴轉多長
		/// </summary>
		float rewindLength = -1;


	};


}}}}







#endif
