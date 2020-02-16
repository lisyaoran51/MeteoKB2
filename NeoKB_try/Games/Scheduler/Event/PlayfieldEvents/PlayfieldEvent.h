#ifndef PLAYFIELD_EVENT_H
#define PLAYFIELD_EVENT_H


#include "../Event.h"

using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers{
namespace Events{
namespace PlayfieldEvents{

	/// <summary>
	/// 用來控制樂器的event
	/// </summary>
	class PlayfieldEvent : public Event {

	public:

		PlayfieldEvent(MTO_FLOAT s, MTO_FLOAT l);

		// 一定要每次都override!!
		virtual string GetTypeName();

	};

}}}}



#endif