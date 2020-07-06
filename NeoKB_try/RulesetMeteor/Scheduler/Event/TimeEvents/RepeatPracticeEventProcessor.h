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
		/// �ثe�Ҧb�p�`
		/// </summary>
		int GetRepeatSection();

		/// <summary>
		/// �˰h����
		/// </summary>
		float GetRewindLength();

	};

}}}}



#endif