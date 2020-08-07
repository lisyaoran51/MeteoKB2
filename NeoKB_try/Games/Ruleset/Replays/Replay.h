#ifndef REPLAY_H
#define REPLAY_H


#include <vector>
#include "ReplayFrame.h"
#include "../../Users/User.h"
#include <mutex>


using namespace std;
using namespace Games::Users;


namespace Games {
namespace Rulesets {
namespace Replays {

	class Replay {

	public:

		mutable mutex replayFramesMutex;

		vector<ReplayFrame*> replayFrames;

		User* user = nullptr;

	};




}}}



#endif