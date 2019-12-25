#ifndef IO_COMMUNICATOR_H
#define IO_COMMUNICATOR_H


#include "../../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../../../Framework/Devices/OutputDevice.h"
#include "../../../../Scheduler/Event/EventProcessor.h"



using namespace Games::Schedulers::Events;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Devices;



namespace Games {
namespace Schedulers {
namespace Events {
namespace IoEvents {
namespace IoCommunicators {

	class IoCommunicatorInterface : public Container {

	public:

		IoCommunicatorInterface();

		virtual int RegisterIoPeripheral(Peripheral* oDevice) = 0;

		virtual int ProcessIO(EventProcessor<Event>* eProcessor) = 0;


	};

	template<typename T>
	class IoCommunicator : public IoCommunicatorInterface {

		int load() {

			LOG(LogLevel::Info) << "IoCommunicator::load() : Start loading config.";

			FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

			if (!f)
				throw runtime_error("int IoCommunicator::load() : FrameworkConfigManager not found in cache.");

			return load(f);
		}

		int load(FrameworkConfigManager* f) {

			//if (!f->Get(FrameworkSetting::StartPitch, &startX))
			//	throw runtime_error("int IoCommunicator::load() : start pitch not found in FrameworkConfigManager.");
			//
			//if (!f->Get(FrameworkSetting::Width, &width))
			//	throw runtime_error("int IoCommunicator::load() : width not found in FrameworkConfigManager.");
			//
			//if (!f->Get(FrameworkSetting::Height, &height))
			//	throw runtime_error("int IoCommunicator::load() : hieght not found in FrameworkConfigManager.");

			return 0;
		}


	public:

		IoCommunicator() : IoCommunicatorInterface(), RegisterType("IoCommunicator") {
			registerLoad(bind((int(IoCommunicator<T>::*)())&IoCommunicator<T>::load, this));
		}

		virtual int RegisterIoPeripheral(Peripheral* oDevice) {
			ioPeripheral = oDevice;
			return 0;
		}

		virtual int ProcessIO(EventProcessor<Event>* eProcessor) {

			return implementProcessIO(Cast<IoEventProcessor<T>>(eProcessor));

		}

	protected:

		Peripheral* ioPeripheral = nullptr;

		virtual int implementProcessIO(IoEventProcessor<T>* eProcessor) = 0;

	};

}}}}}



#endif