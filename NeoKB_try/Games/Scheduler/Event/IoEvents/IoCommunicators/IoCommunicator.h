#ifndef IO_COMMUNICATOR_H
#define IO_COMMUNICATOR_H


#include "../../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../../../Framework/Devices/OutputDevice.h"
#include "../../../../Scheduler/Event/EventProcessor.h"
#include "../../../../../Framework/Output/OutputManager.h"



using namespace Games::Schedulers::Events;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Devices;
using namespace Framework::Output;



namespace Games {
namespace Schedulers {
namespace Events {
namespace IoEvents {
namespace IoCommunicators {

	class IoCommunicatorInterface : public Container {

	public:

		IoCommunicatorInterface();

		virtual int ProcessIO(EventProcessor<Event>* eProcessor) = 0;

	protected:
	};

	template<typename T>
	class IoCommunicator : public IoCommunicatorInterface {

		int load() {

			LOG(LogLevel::Info) << "IoCommunicator::load() : Start loading config.";

			FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

			if (!f)
				throw runtime_error("int IoCommunicator::load() : FrameworkConfigManager not found in cache.");

			OutputManager* o = GetCache<OutputManager>("OutputManager");

			return load(f, o);
		}

		int load(FrameworkConfigManager* f, OutputManager* o) {

			outputManager = o;

			return 0;
		}


	public:

		IoCommunicator() : IoCommunicatorInterface(), RegisterType("IoCommunicator") {
			registerLoad(bind((int(IoCommunicator<T>::*)())&IoCommunicator<T>::load, this));
		}

		virtual int ProcessIO(EventProcessor<Event>* eProcessor) {

			return implementProcessIO(Cast<IoEventProcessor<T>>(eProcessor));

		}

	protected:

		OutputManager* outputManager = nullptr;

		virtual int implementProcessIO(IoEventProcessor<T>* eProcessor) = 0;

	};

}}}}}



#endif