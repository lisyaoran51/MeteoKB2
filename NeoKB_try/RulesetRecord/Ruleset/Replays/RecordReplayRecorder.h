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

		virtual ReplayRecorderReceiver* CreateReceiver();

		virtual int OnKeyDown(pair<RecordAction, int> action);

		virtual int OnKeyUp(RecordAction action);

		virtual int OnButtonDown(RecordAction action);

		virtual int OnButtonUp(RecordAction action);

		virtual int OnKnobTurn(pair<RecordAction, int> action);

		virtual int OnSlide(pair<RecordAction, int> action);

		/* --------------------------RecordReplayRecorder-------------------------- */

		/// <summary>
		/// 只能在遊戲開始時執行，會先放入一個sustain pedal down，之後不放開，就會讓整首歌都一職維持sustain
		///	</summary>
		int SetAutoSustain();

	protected:


		virtual int onKeyDown(pair<RecordAction, int> action);

		virtual int onKeyUp(RecordAction action);

		virtual int onButtonDown(RecordAction action);

		virtual int onButtonUp(RecordAction action);

		virtual int onKnobTurn(pair<RecordAction, int> action);

		virtual int onSlide(pair<RecordAction, int> action);
	};

}}}



#endif