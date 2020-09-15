#include "ReplayRecorder.h"


using namespace Games::Rulesets::Replays;


int ReplayRecorder::load()
{
	TimeController * t = GetCache<TimeController>("TimeController");
	if (!t)
		throw runtime_error("int  ReplayRecorder::load() : TimeController not found in cache.");

	return load(t);
}

int ReplayRecorder::load(TimeController * t)
{
	timeController = t;
	replay = new Replay();
	return 0;
}

ReplayRecorder::ReplayRecorder(): RegisterType("ReplayRecorder")
{
	registerLoad(bind(static_cast<int(ReplayRecorder::*)(void)>(&ReplayRecorder::load), this));
}

ReplayRecorder::~ReplayRecorder()
{
	if(replay != nullptr)
		delete replay;
	replay = nullptr;
}

Replay * ReplayRecorder::GetReplay()
{
	return replay;
}
