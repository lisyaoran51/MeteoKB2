#ifndef IO_EVENT_PROCESSOR_H
#define IO_EVENT_PROCESSOR_H



#include "../../../Scheduler/Event/EventProcessor.h"
#include "IoCommunicator/IoCommunicator.h"


using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::IoEvents::IoCommunicators;



namespace Games {
namespace Schedulers {
namespace Events {
namespace IoEvents {

	class IoEventProcessorInterface : virtual public EventProcessor<Event> {

	public:

		virtual int RegisterIoCommunicator(IoCommunicatorInterface* iCommunicator) = 0;

		virtual int ProcessIo() = 0;

	};

	template<typename T>
	class IoEventProcessor : public IoEventProcessorInterface {

	public:

		virtual int RegisterIoCommunicator(IoCommunicatorInterface* iCommunicator) {
			if (MtoObject::CanCast<IoCommunicator<T>>(iCommunicator)) {
				ioCommunicator = MtoObject::Cast<IoCommunicator<T>>(iCommunicator);
				return 0;
			}
			// TODO: ¼Q¿ù»~
			return -1;
		}


	};



}}}}



#endif
