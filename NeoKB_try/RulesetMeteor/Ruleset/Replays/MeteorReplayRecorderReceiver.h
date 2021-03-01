#ifndef METEOR_REPLAY_RECORDER_RECEIVER_H
#define METEOR_REPLAY_RECORDER_RECEIVER_H


#include "../../../Games/Ruleset/Replays/ReplayRecorderReceiver.h"
#include "../../Input/MeteorInputManager.h"


using namespace Games::Rulesets::Replays;
using namespace Meteor::Input;


namespace Meteor {
namespace Rulesets{
namespace Replays{

	class MeteorReplayRecorderReceiver : public TReplayRecorderReceiver<MeteorAction> {

	public:

		MeteorReplayRecorderReceiver();

	};


}}}







#endif