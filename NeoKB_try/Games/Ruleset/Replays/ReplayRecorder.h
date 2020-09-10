#ifndef REPLAY_RECORDER_H
#define REPLAY_RECORDER_H


#include "../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../Framework/Input/KeyBindings/KeyBindingHandler.h"
#include "../../../Framework/Timing/TimeController.h"
#include "Replay.h"


using namespace Framework::Allocation::Hierachal;
using namespace Framework::Input::KeyBindings;
using namespace Framework::Timing;


namespace Games {
namespace Rulesets {
namespace Replays {

	class ReplayRecorder : public Container {

		int load();

		int load(TimeController* t);

	public:

		ReplayRecorder();
		~ReplayRecorder();

		Replay* GetReplay();

		virtual string GetReplayRecorderVersion() = 0;

	protected:

		Replay* replay = nullptr;

		TimeController* timeController = nullptr;

	};

	template<typename T>
	class TReplayRecorder : public ReplayRecorder, public KeyBindingHandler<T> {

	public:

		TReplayRecorder(): RegisterType("TReplayRecorder"){}

	};


}}}





#endif