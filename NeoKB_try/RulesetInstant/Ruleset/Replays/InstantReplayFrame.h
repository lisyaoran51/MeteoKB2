#ifndef INSTANT_REPLAY_FRAME_H
#define INSTANT_REPLAY_FRAME_H


#include "../../../Games/Ruleset/Replays/ReplayFrame.h"
#include "../../../Instruments/Pitch.h"
#include <utility>
#include "../../Input/InstantInputManager.h"


using namespace Games::Rulesets::Replays;
using namespace Instruments;
using namespace std;
using namespace Instant::Input;


namespace Instant {
namespace Rulesets{
namespace Replays{

	class InstantReplayFrame : public ReplayFrame {

	public:

		InstantReplayFrame(double t, InstantAction mAction, int v, bool down);

		virtual string ToString();

		InstantAction GetInstantAction();

		int GetVolume();

		bool GetIsPressingDown();

	protected:

		InstantAction action = InstantAction::None;

		int volume = -1;

		bool isPressingDown = true;


	};


}}}


#endif