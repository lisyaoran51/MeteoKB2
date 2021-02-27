#ifndef RECORD_REPLAY_RECORDER_H
#define RECORD_REPLAY_RECORDER_H


#include "../../Input/RecordInputManager.h"
#include "../../../Games/Ruleset/Replays/ReplayRecorder.h"



using namespace Games::Rulesets::Replays;
using namespace Record::Input;


namespace Record {
namespace Rulesets{
namespace Replays{

	class RecordReplayRecorder : public TReplayRecorder<RecordAction> {

		

	public:

		RecordReplayRecorder();

		virtual string GetReplayRecorderVersion();

		virtual int OnKeyDown(pair<RecordAction, int> action);

		virtual int OnKeyUp(RecordAction action);

		virtual int OnButtonDown(RecordAction action);

		virtual int OnButtonUp(RecordAction action);

		virtual int OnKnobTurn(pair<RecordAction, int> action);

		virtual int OnSlide(pair<RecordAction, int> action);

	protected:
	};

}}}



#endif