#ifndef RECORD_REPLAY_RECORDER_RECEIVER_H
#define RECORD_REPLAY_RECORDER_RECEIVER_H


#include "../../../Games/Ruleset/Replays/ReplayRecorderReceiver.h"
#include "../../Input/RecordInputManager.h"


using namespace Games::Rulesets::Replays;
using namespace Record::Input;


namespace Record {
namespace Rulesets{
namespace Replays{

	class RecordReplayRecorderReceiver : public TReplayRecorderReceiver<RecordAction> {

	public:

		RecordReplayRecorderReceiver();

	};


}}}







#endif