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

		virtual string GetReplayRecorderVersion();

		virtual ReplayRecorderReceiver* CreateReceiver();

		virtual int OnKeyDown(pair<MeteorAction, int> action);

		virtual int OnKeyUp(MeteorAction action);

		virtual int OnButtonDown(MeteorAction action);

		virtual int OnButtonUp(MeteorAction action);

		virtual int OnKnobTurn(pair<MeteorAction, int> action);

		virtual int OnSlide(pair<MeteorAction, int> action);

		/// <summary>
		/// replay recorder拿步道game的時間，如果遊戲時間有變動，這邊就會無法知道，所以要從receiver把時間送過來
		/// </summary>
		int PassCurrentTime(double cTime);

	protected:

		double lastCurrentTime = -1;

		double thisCurrentTime = -1;

		virtual int update();

		virtual int onKeyDown(pair<MeteorAction, int> action);

		virtual int onKeyUp(MeteorAction action);

		virtual int onButtonDown(MeteorAction action);

		virtual int onButtonUp(MeteorAction action);

		virtual int onKnobTurn(pair<MeteorAction, int> action);

		virtual int onSlide(pair<MeteorAction, int> action);

	};

}}}



#endif