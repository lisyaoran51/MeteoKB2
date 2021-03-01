#include "RecordReplayRecorder.h"

#include "RecordReplayFrame.h"
#include "RecordReplayRecorderReceiver.h"



using namespace Record::Rulesets::Replays;



RecordReplayRecorder::RecordReplayRecorder() : RegisterType("RecordReplayRecorder")
{
	// TODO: 要判斷是auto sustain 還是 sustain pedal(none)控制延音，如果是auto的話要在譜裡面加註
}

string RecordReplayRecorder::GetReplayRecorderVersion()
{
	return string("RecordReplayRecorderV1");
}

ReplayRecorderReceiver * RecordReplayRecorder::CreateReceiver()
{
	return new RecordReplayRecorderReceiver();
}

int RecordReplayRecorder::OnKeyDown(pair<RecordAction, int> action)
{
	return onKeyDown(action);
}

int RecordReplayRecorder::OnKeyUp(RecordAction action)
{
	return onKeyUp(action);
}

int RecordReplayRecorder::OnButtonDown(RecordAction action)
{
	if (isGameControllingPitchState) {
		if (action == RecordAction::LowerOctave || action == RecordAction::RaiseOctave)
			return -1;
	}

	if (isGameControllingSustainPedal) {
		if (action == RecordAction::SustainPedal)
			return -1;
	}
	return onButtonDown(action);
}

int RecordReplayRecorder::OnButtonUp(RecordAction action)
{
	if (isGameControllingSustainPedal) {
		if (action == RecordAction::SustainPedal)
			return -1;
	}
	return onButtonUp(action);
}

int RecordReplayRecorder::OnKnobTurn(pair<RecordAction, int> action)
{
	return 0;
}

int RecordReplayRecorder::OnSlide(pair<RecordAction, int> action)
{
	return 0;
}

int RecordReplayRecorder::SetAutoSustain()
{
	unique_lock<mutex> uLock(replay->replayFramesMutex);
	replay->replayFrames.push_back(new RecordReplayFrame(0, RecordAction::SustainPedal, -1, true));
	return 0;
}

int RecordReplayRecorder::onKeyDown(pair<RecordAction, int> action)
{

	unique_lock<mutex> uLock(replay->replayFramesMutex);
	replay->replayFrames.push_back(new RecordReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action.first, action.second, true));
	return 0;
}

int RecordReplayRecorder::onKeyUp(RecordAction action)
{

	unique_lock<mutex> uLock(replay->replayFramesMutex);
	replay->replayFrames.push_back(new RecordReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action, -1, false));
	return 0;
}

int RecordReplayRecorder::onButtonDown(RecordAction action)
{
	if (action == RecordAction::LowerOctave || action == RecordAction::RaiseOctave || action == RecordAction::SustainPedal) {
		unique_lock<mutex> uLock(replay->replayFramesMutex);
		replay->replayFrames.push_back(new RecordReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action, -1, true));
	}
	return 0;
}

int RecordReplayRecorder::onButtonUp(RecordAction action)
{
	if (action == RecordAction::SustainPedal) {
		unique_lock<mutex> uLock(replay->replayFramesMutex);
		replay->replayFrames.push_back(new RecordReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action, -1, false));
	}
	return 0;
}

int RecordReplayRecorder::onKnobTurn(pair<RecordAction, int> action)
{
	return 0;
}

int RecordReplayRecorder::onSlide(pair<RecordAction, int> action)
{
	return 0;
}

