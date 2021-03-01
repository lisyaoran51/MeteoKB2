#ifndef INSTANT_REPLAY_RECORDER_H
#define INSTANT_REPLAY_RECORDER_H


#include "../../../Games/Ruleset/Replays/ReplayRecorder.h"
#include "../../Input/InstantInputManager.h"



using namespace Games::Rulesets::Replays;
using namespace Instant::Input;


namespace Instant {
namespace Rulesets{
namespace Replays{

	class InstantReplayRecorder : public TReplayRecorder<InstantAction> {

		

	public:

		InstantReplayRecorder();

		virtual string GetReplayRecorderVersion();

		virtual ReplayRecorderReceiver* CreateReceiver();

		virtual int OnKeyDown(pair<InstantAction, int> action);

		virtual int OnKeyUp(InstantAction action);

		virtual int OnButtonDown(InstantAction action);

		virtual int OnButtonUp(InstantAction action);

		virtual int OnKnobTurn(pair<InstantAction, int> action);

		virtual int OnSlide(pair<InstantAction, int> action);

	protected:

		virtual int onKeyDown(pair<InstantAction, int> action);

		virtual int onKeyUp(InstantAction action);

		virtual int onButtonDown(InstantAction action);

		virtual int onButtonUp(InstantAction action);

		virtual int onKnobTurn(pair<InstantAction, int> action);

		virtual int onSlide(pair<InstantAction, int> action);

	};

}}}



#endif