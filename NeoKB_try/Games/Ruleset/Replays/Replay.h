#ifndef REPLAY_H
#define REPLAY_H


#include <vector>
#include "ReplayFrame.h"


using namespace std;


namespace Games {
namespace Rulesets {
namespace Replays {

	class Replay {

		vector<ReplayFrame*> replayFrames;

		User* user = nullptr;

	};




}}}



#endif