#ifndef INSTANT_REPLAY_RECORDER_RECEIVER_H
#define INSTANT_REPLAY_RECORDER_RECEIVER_H


#include "../../../Games/Ruleset/Replays/ReplayRecorderReceiver.h"
#include "../../Input/InstantInputManager.h"


using namespace Games::Rulesets::Replays;
using namespace Instant::Input;


namespace Instant {
namespace Rulesets{
namespace Replays{

	class InstantReplayRecorderReceiver : public TReplayRecorderReceiver<InstantAction> {

	public:

		InstantReplayRecorderReceiver();

	};


}}}







#endif