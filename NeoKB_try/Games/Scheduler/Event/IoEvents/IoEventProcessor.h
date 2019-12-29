#ifndef IO_EVENT_PROCESSOR_H
#define IO_EVENT_PROCESSOR_H



#include "../../../Scheduler/Event/EventProcessor.h"
#include "IoCommunicators/IoCommunicator.h"


using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::IoEvents::IoCommunicators;



namespace Games {
namespace Schedulers {
namespace Events {
namespace IoEvents {

	enum class IoTransferType {
		Once,		// �u�Ǥ@��
		Timed,		// �ǩT�w����
		Continuous,	// �@����
	};

	class IoEventProcessorInterface : virtual public EventProcessor<Event> {

	public:

		virtual int RegisterIoCommunicator(IoCommunicatorInterface* iCommunicator) = 0;

		virtual int ProcessIo() = 0;

		virtual int SetIsTransfered() = 0;

		virtual bool GetIsTransferable() = 0;

	};

	template<typename T>
	class IoEventProcessor : public IoEventProcessorInterface {

	public:

		virtual int RegisterIoCommunicator(IoCommunicatorInterface* iCommunicator) {
			if (MtoObject::CanCast<IoCommunicator<T>>(iCommunicator)) {
				ioCommunicator = MtoObject::Cast<IoCommunicator<T>>(iCommunicator);
				return 0;
			}
			// TODO: �Q���~
			return -1;
		}

		virtual int ProcessIo() {
			if (ioCommunicator)
				ioCommunicator->ProcessIO(this);
			return 0;
		}

		T* GetIoEvent() { return dynamic_cast<T*>(event); }

		virtual int SetIsTransfered() {
			if (ioTransferType == IoTransferType::Once)
				isTransferable = false;
			else if (ioTransferType == IoTransferType::Timed) {
				transferTime--;
				if (transferTime == 0)
					isTransferable = false;
			}
		}

		virtual bool GetIsTransferable() {
			return isTransferable;
		}

	protected:

		bool isTransferable = true;

		int transferTime = 1;

		IoTransferType ioTransferType = IoTransferType::Once;

		IoCommunicator<T>* ioCommunicator = nullptr;

	};



}}}}



#endif
