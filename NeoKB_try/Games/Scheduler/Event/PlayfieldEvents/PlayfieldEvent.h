#ifndef PLAYFIELD_EVENT_H
#define PLAYFIELD_EVENT_H


#include "../Event.h"

using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers{
namespace Events{
namespace PlayfieldEvents{

	/// <summary>
	/// �Ψӱ���־���event
	/// </summary>
	class PlayfieldEvent : public Event {

	public:

		PlayfieldEvent(MTO_FLOAT s, MTO_FLOAT l);

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	};

}}}}



#endif