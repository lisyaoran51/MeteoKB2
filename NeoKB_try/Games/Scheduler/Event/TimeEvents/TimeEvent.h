#ifndef TIME_EVENT_H
#define TIME_EVENT_H


#include "../Event.h"

using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers{
namespace Events{
namespace TimeEvents{

	/// <summary>
	/// �Ψӱ��ܮɶ���event
	/// </summary>
	class TimeEvent : public Event {

	public:

		TimeEvent(MTO_FLOAT s, MTO_FLOAT l);

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	};

}}}}



#endif