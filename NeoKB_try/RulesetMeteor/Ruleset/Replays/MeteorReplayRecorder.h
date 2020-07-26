#ifndef METEOR_REPLAY_RECORDER_H
#define METEOR_REPLAY_RECORDER_H


#include "../../../Games/Ruleset/Replays/ReplayRecorder.h"
#include "../../Input/MeteorInputManager.h"



using namespace Games::Rulesets::Replays;
using namespace Meteor::Input;


namespace Meteor {
namespace Rulesets{
namespace Replays{

	class MeteorReplayRecorder : public TReplayRecorder<MeteorAction> {

	public:

		MeteorReplayRecorder();

		virtual string* GetReplayRecorderVersion();

		virtual int OnKeyDown(pair<MeteorAction, int> action);

		virtual int OnKeyUp(MeteorAction action);

		virtual int OnButtonDown(MeteorAction action);

		virtual int OnButtonUp(MeteorAction action);

		virtual int OnKnobTurn(pair<MeteorAction, int> action);

		virtual int OnSlide(pair<MeteorAction, int> action);

	protected:



	};

}}}



#endif