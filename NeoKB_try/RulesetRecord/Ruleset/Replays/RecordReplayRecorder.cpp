#include "RecordReplayRecorder.h"

#include "RecordReplayFrame.h"



using namespace Record::Rulesets::Replays;



RecordReplayRecorder::RecordReplayRecorder() : RegisterType("RecordReplayRecorder")
{
}

string RecordReplayRecorder::GetReplayRecorderVersion()
{
	return string("RecordReplayRecorderV1");
}

int RecordReplayRecorder::OnKeyDown(pair<RecordAction, int> action)
{

	unique_lock<mutex> uLock(replay->replayFramesMutex);
	replay->replayFrames.push_back(new RecordReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action.first, action.second, true));
	return 0;
}

int RecordReplayRecorder::OnKeyUp(RecordAction action)
{

	unique_lock<mutex> uLock(replay->replayFramesMutex);
	replay->replayFrames.push_back(new RecordReplayFrame(timeController->GetControllableClock()->GetCurrentTime(), action, -1, false));
	return 0;
}

int RecordReplayRecorder::OnButtonDown(RecordAction action)
{
	return 0;
}

int RecordReplayRecorder::OnButtonUp(RecordAction action)
{
	return 0;
}

int RecordReplayRecorder::OnKnobTurn(pair<RecordAction, int> action)
{
	return 0;
}

int RecordReplayRecorder::OnSlide(pair<RecordAction, int> action)
{
	return 0;
}

