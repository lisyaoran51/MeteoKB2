#ifndef TIME_EVENT_H
#define TIME_EVENT_H


#include "../Event.h"

using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers{
namespace Events{
namespace TimeEvents{

	/// <summary>
	/// 用來控至時間的event
	/// </summary>
	class TimeEvent : public Event {

	public:

		TimeEvent(MTO_FLOAT s, MTO_FLOAT l);

		// 一定要每次都override!!
		virtual string GetTypeName();

	};

}}}}



#endif