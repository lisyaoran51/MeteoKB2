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

		// �@�w�n�C����override!!
		virtual string GetTypeName();

		int GetRepeatSection();

		float GetRewindLength();

	protected:

		/// <summary>
		/// ���ƪ��p�`
		/// </summary>
		int repeatSection = -1;

		/// <summary>
		/// �n���e�j��h��
		/// </summary>
		float rewindLength = -1;


	};


}}}}







#endif
