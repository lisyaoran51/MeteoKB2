#include "RecordReplayRecorder.h"

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
	return 0;
}

int RecordReplayRecorder::OnKeyUp(RecordAction action)
{
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