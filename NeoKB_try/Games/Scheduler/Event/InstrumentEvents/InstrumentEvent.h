#ifndef INSTRUMENT_EVENT_H
#define INSTRUMENT_EVENT_H


#include "../Event.h"

using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers{
namespace Events{
namespace InstrumentEvents{

	/// <summary>
	/// �Ψӱ���־���event
	/// </summary>
	class InstrumentEvent : public Event {

	public:

		InstrumentEvent(MTO_FLOAT s, MTO_FLOAT l);

	};

}}}}




#endif