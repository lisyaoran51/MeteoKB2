#include "MeteorReplayRecorder.h"

#include "MeteorReplayFrame.h"

using namespace Meteor::Rulesets::Replays;



MeteorReplayRecorder::MeteorReplayRecorder() : RegisterType("MeteorReplayRecorder")
{
}

string MeteorReplayRecorder::GetReplayRecorderVersion()
{
	return string("MeteorReplayRecorderV1");
}

int MeteorReplayRecorder::OnKeyDown(pair<MeteorAction, int> action)
{
	if (lastCurrentTime == thisCurrentTime)
		return -1;

	unique_lock<mutex> uLock(replay->replayFramesMutex);
	replay->replayFrames.push_back(new MeteorReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action.first, action.second, true));
	return 0;
}

int MeteorReplayRecorder::OnKeyUp(MeteorAction action)
{
	if (lastCurrentTime == thisCurrentTime)
		return -1;

	unique_lock<mutex> uLock(replay->replayFramesMutex);
	replay->replayFrames.push_back(new MeteorReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action, -1, false));
	return 0;
}

int MeteorReplayRecorder::OnButtonDown(MeteorAction action)
{
	return 0;
}

int MeteorReplayRecorder::OnButtonUp(MeteorAction action)
{
	return 0;
}

int MeteorReplayRecorder::OnKnobTurn(pair<MeteorAction, int> action)
{
	return 0;
}

int MeteorReplayRecorder::OnSlide(pair<MeteorAction, int> action)
{
	return 0;
}

int MeteorReplayRecorder::update()
{
	lastCurrentTime = thisCurrentTime;
	thisCurrentTime = timeController->GetControllableClock()->GetCurrentTime();

	if (lastCurrentTime > thisCurrentTime) {
		// 清除倒退的音
		for (int i = 0; i < replay->replayFrames.size(); i++) {
			if (replay->replayFrames[i]->GetStartTime() > thisCurrentTime) {
				unique_lock<mutex> uLock(replay->replayFramesMutex);
				// TODO: 要lock一下，避免race condition
				delete replay->replayFrames[i];
				replay->replayFrames.erase(replay->replayFrames.begin() + i);
				uLock.unlock();
				i--;
				// TODO: 檢查有沒有key up的replay，有的話要加回去，不過改一下時間
				// 也可以不加回去，就變成只有按下，但是沒有抬起
			}
		}
	}

	return 0;
}
