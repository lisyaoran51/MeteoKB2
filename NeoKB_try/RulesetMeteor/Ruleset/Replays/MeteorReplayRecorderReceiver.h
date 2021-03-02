#ifndef METEOR_REPLAY_RECORDER_RECEIVER_H
#define METEOR_REPLAY_RECORDER_RECEIVER_H


#include "../../../Games/Ruleset/Replays/ReplayRecorderReceiver.h"
#include "../../Input/MeteorInputManager.h"
#include "MeteorReplayRecorder.h"


using namespace Games::Rulesets::Replays;
using namespace Meteor::Input;


namespace Meteor {
namespace Rulesets{
namespace Replays{

	class MeteorReplayRecorderReceiver : public TReplayRecorderReceiver<MeteorAction> {

		int load();

	public:

		MeteorReplayRecorderReceiver();


	protected:


		MeteorReplayRecorder* meteorReplayRecorder = nullptr;

		virtual int update();

	};


}}}







#endif