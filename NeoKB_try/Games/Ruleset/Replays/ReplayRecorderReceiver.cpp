#include "ReplayRecorderReceiver.h"


using namespace Games::Rulesets::Replays;


int ReplayRecorderReceiver::load()
{
	LOG(LogLevel::Info) << "ReplayRecorderReceiver::load : connecting receiver to recorder ... ";

	ReplayRecorder * r = GetCache<ReplayRecorder>("ReplayRecorder");
	if (!r)
		throw runtime_error("ReplayRecorderReceiver::load() : ReplayRecorder not found in cache.");

	return load(r);
}

int ReplayRecorderReceiver::load(ReplayRecorder * r)
{
	replayRecorder = r;

	return 0;
}

ReplayRecorderReceiver::ReplayRecorderReceiver(): RegisterType("ReplayRecorderReceiver")
{
	registerLoad(bind((int(ReplayRecorderReceiver::*)())&ReplayRecorderReceiver::load, this));
}
