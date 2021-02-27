#ifndef RECORD_REPLAY_FRAME_H
#define RECORD_REPLAY_FRAME_H


#include "../../../Games/Ruleset/Replays/ReplayFrame.h"
#include "../../../Instruments/Pitch.h"
#include <utility>
#include "../../Input/RecordInputManager.h"


using namespace Games::Rulesets::Replays;
using namespace Instruments;
using namespace std;
using namespace Record::Input;


namespace Record {
namespace Rulesets{
namespace Replays{

	class RecordReplayFrame : public ReplayFrame {

	public:

		RecordReplayFrame(double t, RecordAction mAction, int v, bool down);

		virtual string ToString();

		RecordAction GetRecordAction();

		int GetVolume();

		bool GetIsPressingDown();

	protected:

		RecordAction action = RecordAction::None;

		int volume = -1;

		bool isPressingDown = true;


	};


}}}


#endif