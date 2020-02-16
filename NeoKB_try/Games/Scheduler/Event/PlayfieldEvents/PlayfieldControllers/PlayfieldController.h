#ifndef PLAYFIELD_CONTROLLER_H
#define PLAYFIELD_CONTROLLER_H



#include "../../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../../Scheduler/Event/EventProcessor.h"


using namespace Games::Schedulers::Events;
using namespace Framework::Allocation::Hierachal;

namespace Games {
namespace Scenes{
namespace Play{
	class Playfield;
}}}


using namespace Games::Scenes::Play;


namespace Games {
namespace Schedulers{
namespace Events{
namespace PlayfieldEvents{
namespace PlayfieldControllers{

	class PlayfieldControllerInterface : public Container {

	public:

		PlayfieldControllerInterface();

		virtual int LazyConstruct(Playfield* p) = 0;

		virtual int ControlPlayfield(EventProcessor<Event>* eProcessor) = 0;

	protected:
	};


	template<typename T>
	class PlayfieldController : public PlayfieldControllerInterface {

		int load() {

			LOG(LogLevel::Info) << "PlayfieldController::load() : Start loading config.";

			FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

			if (!f)
				throw runtime_error("int PlayfieldController::load() : FrameworkConfigManager not found in cache.");

			return load(f);
		}

		int load(FrameworkConfigManager* f) {

			return 0;
		}


	public:

		PlayfieldController() : PlayfieldControllerInterface(), RegisterType("PlayfieldController") {
			registerLoad(bind((int(PlayfieldController<T>::*)())&PlayfieldController<T>::load, this));
		}

		virtual int LazyConstruct(Playfield* p) {
			playfield = p;
			return 0;
		}

		virtual int ControlPlayfield(EventProcessor<Event>* eProcessor) {

			return implementControlPlayfield(eProcessor);

		}

	protected:

		virtual int implementControlPlayfield(EventProcessor<Event>* eProcessor) = 0;

		Playfield* playfield = nullptr;

	};

}}}}}



#endif