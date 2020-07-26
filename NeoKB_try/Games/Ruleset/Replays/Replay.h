#ifndef REPLAY_H
#define REPLAY_H


#include <vector>
#include "ReplayFrame.h"
#include "../../Users/User.h"


using namespace std;
using namespace Games::Users;


namespace Games {
namespace Rulesets {
namespace Replays {

	class Replay {

	public:

		vector<ReplayFrame*> replayFrames;

		User* user = nullptr;

	};




}}}



#endif