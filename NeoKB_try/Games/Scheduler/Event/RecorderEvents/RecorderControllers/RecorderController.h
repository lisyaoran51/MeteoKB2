#ifndef RECORDER_CONTROLLER_H
#define RECORDER_CONTROLLER_H


#include "../../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../EventProcessor.h"
#include "../../../../Ruleset/Replays/ReplayRecorder.h"
#include "../../../../../Framework/Timing/TimeController.h"



using namespace Framework::Allocation::Hierachal;
using namespace Games::Schedulers::Events;
using namespace Games::Rulesets::Replays;
using namespace Framework::Timing;



namespace Games {
namespace Schedulers{
namespace Events{
namespace RecorderEvents{
namespace RecorderControllers{

	class RecorderControllerInterface : public Container {

	public:

		RecorderControllerInterface();

		virtual int AddReplay(EventProcessor<Event>* e) = 0;

	};

	template<typename T>
	class RecorderController : public RecorderControllerInterface {

		int load() {

			LOG(LogLevel::Info) << "IoCommunicator::load() : Start loading config.";

			FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

			ReplayRecorder* r = GetCache<ReplayRecorder>("ReplayRecorder");

			TimeController* t = GetCache<TimeController>("TimeController");

			return load(f, r, t);
		}

		int load(FrameworkConfigManager* f, ReplayRecorder* r, TimeController* t) {

			replayRecorder = r;

			timeController = t;

			return 0;
		}

	public:

		RecorderController(): RegisterType("RecorderController") {
			registerLoad(bind((int(RecorderController<T>::*)())&RecorderController<T>::load, this));
		}

		virtual int AddReplay(EventProcessor<Event>* e) {
			return implementAddReplay(e);
		}


	protected:

		ReplayRecorder* replayRecorder = nullptr;

		TimeController* timeController = nullptr;

		virtual int implementAddReplay(EventProcessor<Event>* e) = 0;

	};

}}}}}

#endif