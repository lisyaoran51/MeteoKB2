#ifndef REPEAT_PRACTICE_EVENT_PROCESSOR_H
#define REPEAT_PRACTICE_EVENT_PROCESSOR_H


#include "../../../../Games/Scheduler/Event/TimeEvents/TimeEventProcessor.h"
#include "RepeatPracticeEvent.h"


using namespace Games::Schedulers::Events::TimeEvents;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace TimeEvents {

	class RepeatPracticeEventProcessor : public TimeEventProcessor<RepeatPracticeEvent> {

	public:

		RepeatPracticeEventProcessor();

		/// <summary>
		/// 目前所在小節
		/// </summary>
		int GetRepeatSection();

		/// <summary>
		/// 倒退長度
		/// </summary>
		float GetRewindLength();

	};

}}}}



#endif