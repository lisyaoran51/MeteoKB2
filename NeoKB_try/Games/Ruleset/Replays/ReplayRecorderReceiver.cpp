#include "ReplayRecorderReceiver.h"


using namespace Games::Rulesets::Replays;


int ReplayRecorderReceiver::load()
{
	LOG(LogLevel::Info) << "ReplayRecorderReceiver::load : connecting receiver to recorder ... ";

	ReplayRecorder * r = GetCache<ReplayRecorder>("ReplayRecorder");
	if (!r)
		throw runtime_error("ReplayRecorderReceiver::load() : ReplayRecorder not found in cache.");

	TimeController * t = GetCache<TimeController>("TimeController");
	if (!t)
		throw runtime_error("ReplayRecorderReceiver::load() : TimeController not found in cache.");

	return load(r, t);
}

int ReplayRecorderReceiver::load(ReplayRecorder * r, TimeController * t)
{
	isPresent = true;

	replayRecorder = r;



	t->AddOnPause(this, [=]() {
		isRecording = false;
		return 0;
	}, "ReplayRecorderReceiver::Lambda_HandleOnPause");

	t->AddOnPauseEnd(this, [=]() {
		isRecording = true;
		return 0;
	}, "ReplayRecorderReceiver::Lambda_HandleOnPauseEnd");


	return 0;
}

ReplayRecorderReceiver::ReplayRecorderReceiver(): RegisterType("ReplayRecorderReceiver")
{
	isInputable = true;
	registerLoad(bind((int(ReplayRecorderReceiver::*)())&ReplayRecorderReceiver::load, this));
}
