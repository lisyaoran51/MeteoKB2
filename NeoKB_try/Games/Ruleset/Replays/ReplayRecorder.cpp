#include "ReplayRecorder.h"

#include "../Ruleset.h"


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
	GetDependencies()->Cache<ReplayRecorder>(this, "ReplayRecorder");

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

	GetDependencies()->DeleteCache("ReplayRecorder");

	if (keyBindings != nullptr) {
		for (int i = 0; i < keyBindings->size(); i++) {
			delete keyBindings->at(i);
		}

		keyBindings->clear();
		delete keyBindings;
		keyBindings = nullptr;
	}
		
}

Replay * ReplayRecorder::GetReplay()
{
	return replay;
}

int ReplayRecorder::SetDefaultKeyBindings(RulesetInfo * rInfo)
{
	if (keyBindings != nullptr)
		return -1;

	Ruleset* ruleset = rInfo->CreateRuleset();
	keyBindings = ruleset->GetDefaultkeyBindings();
	delete ruleset;

	return 0;
}

int ReplayRecorder::SetGameControllingPitchState(bool value)
{
	isGameControllingPitchState = value;
	return 0;
}

int ReplayRecorder::SetGameControllingSustainPedal(bool value)
{
	isGameControllingSustainPedal = value;
	return 0;
}
