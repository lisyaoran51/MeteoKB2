#include "MeteorReplayRecorder.h"

#include "MeteorReplayFrame.h"
#include "MeteorReplayRecorderReceiver.h"

using namespace Meteor::Rulesets::Replays;




MeteorReplayRecorder::MeteorReplayRecorder() : RegisterType("MeteorReplayRecorder")
{
	isGameControllingPitchState = true;
}

string MeteorReplayRecorder::GetReplayRecorderVersion()
{
	return string("MeteorReplayRecorderV1");
}

ReplayRecorderReceiver * MeteorReplayRecorder::CreateReceiver()
{
	return new MeteorReplayRecorderReceiver();
}

int MeteorReplayRecorder::OnKeyDown(pair<MeteorAction, int> action)
{
	
	return onKeyDown(action);
}

int MeteorReplayRecorder::OnKeyUp(MeteorAction action)
{
	
	return onKeyUp(action);
}

int MeteorReplayRecorder::OnButtonDown(MeteorAction action)
{
	if (isGameControllingPitchState) {
		if (action == MeteorAction::LowerOctave || action == MeteorAction::RaiseOctave)
			return -1;
	}

	if (isGameControllingSustainPedal) {
		if (action == MeteorAction::SustainPedal)
			return -1;
	}
	return onButtonDown(action);
}

int MeteorReplayRecorder::OnButtonUp(MeteorAction action)
{

	if (isGameControllingSustainPedal) {
		if (action == MeteorAction::SustainPedal)
			return -1;
	}
	return onButtonUp(action);
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
		// �M���˰h����
		for (int i = 0; i < replay->replayFrames.size(); i++) {
			if (replay->replayFrames[i]->GetStartTime() > thisCurrentTime) {
				unique_lock<mutex> uLock(replay->replayFramesMutex);
				// TODO: �nlock�@�U�A�קKrace condition
				delete replay->replayFrames[i];
				replay->replayFrames.erase(replay->replayFrames.begin() + i);
				uLock.unlock();
				i--;
				// TODO: �ˬd���S��key up��replay�A�����ܭn�[�^�h�A���L��@�U�ɶ�
				// �]�i�H���[�^�h�A�N�ܦ��u�����U�A���O�S����_
			}
		}
	}

	return 0;
}

int MeteorReplayRecorder::onKeyDown(pair<MeteorAction, int> action)
{

	LOG(LogLevel::Debug) << "MeteorReplayRecorder::onKeyDown : record key [" << (int)action.first << "]." << lastCurrentTime << " " << thisCurrentTime;

	if (lastCurrentTime >= thisCurrentTime)
		return -1;

	unique_lock<mutex> uLock(replay->replayFramesMutex);
	replay->replayFrames.push_back(new MeteorReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action.first, action.second, true));
	return 0;
}

int MeteorReplayRecorder::onKeyUp(MeteorAction action)
{
	if (lastCurrentTime >= thisCurrentTime)
		return -1;

	unique_lock<mutex> uLock(replay->replayFramesMutex);
	replay->replayFrames.push_back(new MeteorReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action, -1, false));
	return 0;
}

int MeteorReplayRecorder::onButtonDown(MeteorAction action)
{
	if (lastCurrentTime >= thisCurrentTime)
		return -1;

	if (action == MeteorAction::LowerOctave || action == MeteorAction::RaiseOctave || action == MeteorAction::SustainPedal) {
		unique_lock<mutex> uLock(replay->replayFramesMutex);
		replay->replayFrames.push_back(new MeteorReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action, -1, true));
	}
	
	return 0;
}

int MeteorReplayRecorder::onButtonUp(MeteorAction action)
{
	if (lastCurrentTime >= thisCurrentTime)
		return -1;

	if (action == MeteorAction::SustainPedal) {
		unique_lock<mutex> uLock(replay->replayFramesMutex);
		replay->replayFrames.push_back(new MeteorReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action, -1, false));
	}
	return 0;
}

int MeteorReplayRecorder::onKnobTurn(pair<MeteorAction, int> action)
{
	return 0;
}

int MeteorReplayRecorder::onSlide(pair<MeteorAction, int> action)
{
	return 0;
}
