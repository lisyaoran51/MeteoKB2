#include "TimeController.h"

//#include "../../Scheduler/Event/ControlPoints/NoteControlPoint.h"




using namespace Framework::Timing;
using namespace Framework::Timing::SpeedAdjusters;
//using namespace Games::Schedulers::Events::ControlPoints;  // 還沒有要寫這個



int TimeController::load()
{
	return 0;
}

int TimeController::update()
{
	LOG(LogLevel::Finest) << "TimeController::update() : speed adjuster = [" << speedAdjuster << "], controllable clock = [" << controllableClock << "].";

	if (controllableClock == nullptr || speedAdjuster == nullptr)
		return 0;

	speedAdjuster->ProcessFrame(GetClock()->GetElapsedFrameTime());

	if (speedAdjuster->GetIsAdjustingTime()) {
		double timeToAdjust = speedAdjuster->GetAdjustFrameTime();
		
		// 這邊應該是，不管目前速度多快，調整時間的速度都是固定的，不會速度快的時候就調的快
		controllableClock->Seek(controllableClock->GetCurrentTime() + timeToAdjust/* * controllableClock->GetRate()*/);
	}
	else if (speedAdjuster->GetIsFreezingTime()) {
		Pause();
	}

	return 0;
}

/* 暫時不寫這段，以後響到要怎麼寫再回來改
int TimeController::getTempSection()
{
	for (int i = 0; i < sectionStartTime.size(); i++) {
		if (sectionStartTime[i] > framedClock->GetCurrentTime())
			return i - 1;
	}

	return sectionStartTime.size() - 1;
}

int TimeController::getTempPart()
{
	for (int i = 0; i < partStartTime.size(); i++) {
		if (partStartTime[i] > framedClock->GetCurrentTime())
			return i - 1;
	}

	return partStartTime.size() - 1;
}
*/

TimeController::TimeController() : RegisterType("TimeController")
{
	registerLoad(bind(static_cast<int(TimeController::*)(void)>(&TimeController::load), this));

	isInputable = true;
	isPresent = true;
}

int TimeController::SetControllableClock(AdjustableClock * cClock)
{
	controllableClock = cClock;
	return 0;
}

int TimeController::SetSpeedAdjuster(SpeedAdjuster * sAdjuster)
{
	// TODO: 做一個thread safe lock再刪
	if(speedAdjuster != nullptr)
		delete speedAdjuster;
	speedAdjuster = sAdjuster;
	

	speedAdjuster->AddOnAdjustFreeze(this, bind(&TimeController::SetAllChildsIsMaskedForTrigger, this),
		"TimeController::SetAllChildsIsMaskedForTrigger");

	speedAdjuster->AddOnAdjustFreezeEnd(this, bind(&TimeController::RecoverAllChildsIsMaskedForTrigger, this),
		"TimeController::RecoverAllChildsIsMaskedForTrigger");

	speedAdjuster->AddOnAdjustFreezeEnd(this, bind(&TimeController::Resume, this),
		"TimeController::Resume");

	return 0;
}

int TimeController::JumpTo(double seekTime)
{
	controllableClock->Seek(seekTime);
	return 0;
}

int TimeController::JumpToWithSpeedAdjust(double seekTime)
{
	speedAdjuster->SetSeekTime(seekTime);
	return 0;
}

int TimeController::Pause()
{
	controllableClock->Stop();
	isPaused = true;

	return 0;
}

int TimeController::Resume()
{
	if (isAdjustAfterPause)
		return 0;
	controllableClock->Start();
	isPaused = false;
	isWaitingFreeze = false;
	return 0;
}

int TimeController::SetRate(double r)
{
	if (r == 0)
		throw invalid_argument("TimeController::SetRate() : 0 is not available for rate.");
	rate = r;
	controllableClock->SetRate(r);
	return 0;
}

double TimeController::GetRate()
{
	
	return rate = controllableClock->GetRate();
}

bool TimeController::GetIsPaused()
{
	return isPaused;
}

/* 暫時不寫這段，以後響到要怎麼寫再回來改
int TimeController::ImportWorkingSm(WorkingSm * workingSm)
{
	// TODO: 這邊要去分析整個sm，然後把每個小節的位置抓出來，每個段落的位置的抓出來，然後放進vector裡
	vector<Event*>* events = workingSm->GetSm()->GetEvents();

	int tempSection = 0;
	int tempPart = 0;
	sectionStartTime.push_back(0);
	partStartTime.push_back(0);

	for (int i = 0; i < events->size(); i++) {
		if (dynamic_cast<NoteControlPoint*>(events->at(i))->GetSectionIndex() > tempSection) {
			tempSection++;
			sectionStartTime.push_back(events->at(i)->GetStartTime());
		}
		if (dynamic_cast<NoteControlPoint*>(events->at(i))->GetPartIndex() > tempPart) {
			tempPart++;
			partStartTime.push_back(events->at(i)->GetStartTime());
		}

	}


	return 0;
}
*/