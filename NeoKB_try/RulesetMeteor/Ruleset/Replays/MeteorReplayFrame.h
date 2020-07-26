#ifndef METEOR_REPLAY_FRAME_H
#define METEOR_REPLAY_FRAME_H


#include "../../../Games/Ruleset/Replays/ReplayFrame.h"
#include "../../../Instruments/Pitch.h"
#include <utility>
#include "../../Input/MeteorInputManager.h"


using namespace Games::Rulesets::Replays;
using namespace Instruments;
using namespace std;
using namespace Meteor::Input;


namespace Meteor {
namespace Rulesets{
namespace Replays{

	class MeteorReplayFrame : public ReplayFrame {

	public:

		MeteorReplayFrame(double t, MeteorAction mAction, int v, bool down);

		virtual string ToString();

		MeteorAction GetMeteorAction();

		int GetVolume();

		bool GetIsPressingDown();

	protected:

		MeteorAction action = MeteorAction::None;

		int volume = -1;

		bool isPressingDown = true;


	};


}}}


#endif