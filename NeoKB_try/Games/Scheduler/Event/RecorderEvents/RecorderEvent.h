#ifndef RECORDER_EVENT_H
#define RECORDER_EVENT_H


#include "../Event.h"

using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers{
namespace Events{
namespace RecorderEvents{

	/// <summary>
	/// �Ψӱ���recorder��event
	/// </summary>
	class RecorderEvent : public Event {

	public:

		RecorderEvent(MTO_FLOAT s, MTO_FLOAT l);

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	};

}}}}




#endif