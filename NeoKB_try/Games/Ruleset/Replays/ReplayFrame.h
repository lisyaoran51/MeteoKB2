#ifndef REPLAY_FRAME_H
#define REPLAY_FRAME_H


namespace Games {
namespace Rulesets {
namespace Replays {

	class ReplayFrame {

	public:

		ReplayFrame(double t);

		virtual string ToString() = 0;

		double GetStartTime();

	protected:

		double time = -1;

	};




}}}



#endif