#ifndef IO_EVENT_H
#define IO_EVENT_H


#include "../Event.h"


using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers {
namespace Events {
namespace IoEvents {

	/// <summary>
	/// �|��D�O�H�~���˸mio��event
	/// </summary>
	class IoEvent : public Event {

	public:

		IoEvent(MTO_FLOAT s, MTO_FLOAT l);

	protected:

	};



}}}}









#endif
