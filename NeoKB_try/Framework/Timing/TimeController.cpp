#include "TimeController.h"

#include <iomanip>
//#include "../../Scheduler/Event/ControlPoints/NoteControlPoint.h"



using namespace std;
using namespace Framework::Timing;
using namespace Framework::Timing::SpeedAdjusters;
//using namespace Games::Schedulers::Events::ControlPoints;  // 還沒有要寫這個



int TimeController::load()
{

	GetDependencies()->Cache<TimeController>(this, "TimeController");

	return 0;
}

int TimeController::update()
{
	

	if (controllableClock == nullptr || speedAdjuster == nullptr)
		return 0;

	LOG(LogLevel::Finest) << "TimeController::update() : speed adjuster sync to source clock time [" << fixed << setprecision(5) << GetClock()->GetElapsedFrameTime() << "].";
	speedAdjuster->ProcessFrame(GetClock()->GetElapsedFrameTime());
	LOG(LogLevel::Finest) << "TimeController::update() : speed adjuster process over.";

	if (speedAdjuster->GetIsAdjustingTime()) {
		LOG(LogLevel::Finest) << "TimeController::update() : speed adjuster is adjusting time to [" << fixed << setprecision(5) << speedAdjuster->GetAdjustFrameTime() << "].";
		double timeToAdjust = speedAdjuster->GetAdjustFrameTime();
		
		LOG(LogLevel::Finest) << "TimeController::update() :controllable clock before adjust [" << fixed << setprecision(5) << controllableClock->GetCurrentTime() << "].";
		// 這邊應該是，不管目前速度多快，調整時間的速度都是固定的，不會速度快的時候就調的快
		controllableClock->Seek(controllableClock->GetCurrentTime() + timeToAdjust/* * controllableClock->GetRate()*/);
		LOG(LogLevel::Finest) << "TimeController::update() :controllable clock after adjust [" << fixed << setprecision(5) << controllableClock->GetCurrentTime() << "].";

		// 檢查是不是已經adjust完畢了
		if (speedAdjuster->GetIsLastAdjustingTime()) {

			Resume();

		}

	}
	else if (speedAdjuster->GetIsFreezingTime()) {
		Pause();
	}
	else if (speedAdjuster->GetIsLastFreezingTime()) {

		Resume();
	}

	if (checkIsGameOver()) {
		LOG(LogLevel::Info) << "TimeController::update() : game over.";
		onGameOver.TriggerThenClear();
	}


	LOG(LogLevel::Finest) << "TimeController::update() : end.";

	return 0;
}

bool TimeController::checkIsGameOver()
{
	return false;
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

TimeController::~TimeController()
{
	LOG(LogLevel::Depricated) << "TimeController::~TimeController() : deleting from cache.";

	GetDependencies()->DeleteCache("TimeController");

	LOG(LogLevel::Depricated) << "TimeController::~TimeController() : deleting from cache over.";

}

int TimeController::SetControllableClock(AdjustableClock * cClock)
{
	controllableClock = cClock;
	return 0;
}

AdjustableClock * TimeController::GetControllableClock()
{
	return controllableClock;
}

int TimeController::SetSpeedAdjuster(SpeedAdjuster * sAdjuster)
{
	// TODO: 做一個thread safe lock再刪
	if(speedAdjuster != nullptr)
		delete speedAdjuster;
	speedAdjuster = sAdjuster;
	
	
	speedAdjuster->AddOnAdjustFreeze(this, bind(&TimeController::SetAllChildsIsMaskedForTrigger, this),
		"TimeController::SetAllChildsIsMaskedForTrigger");

	speedAdjuster->AddOnAdjustFreeze(controllableClock, bind(&AdjustableClock::Stop, controllableClock),
		"AdjustableClock::Stop");

	speedAdjuster->AddOnAdjustFreezeEnd(this, bind(&TimeController::RecoverAllChildsIsMaskedForTrigger, this),
		"TimeController::RecoverAllChildsIsMaskedForTrigger");
	
	speedAdjuster->AddOnAdjustFreezeEnd(controllableClock, bind(&AdjustableClock::Start, controllableClock),
		"AdjustableClock::Start");

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
	if (isPaused)
		return 0;
	LOG(LogLevel::Debug) << "TimeController::Pause() : stop controllable clock.";
	//controllableClock->Stop(); 這行在speed adjuster 的on pause裡面
	speedAdjuster->Pause();
	isPaused = true;

	return 0;
}

int TimeController::Resume()
{
	if (isAdjustAfterPause)
		return 0;
	speedAdjuster->Resume();
	//controllableClock->Start(); 擺在speed adjuster的on freeze end裡面
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

int TimeController::SetIsAllowSeek(bool iAllowSeek)
{
	isAllowSeek = iAllowSeek;
	return 0;
}

bool TimeController::GetIsAllowSeek()
{
	return isAllowSeek;
}

TimeControllerState TimeController::GetTimeControllerState()
{
	switch (speedAdjuster->GetSpeedAdjusterState()) {
	case SpeedAdjusterState::Backward:
		return TimeControllerState::Backward;
		break;
	case SpeedAdjusterState::FastForward:
		return TimeControllerState::FastForward;
		break;
	case SpeedAdjusterState::Freezing:
		return TimeControllerState::Freezing;
		break;
	case SpeedAdjusterState::Normal:
		return TimeControllerState::Normal;
		break;
	}
	return TimeControllerState::None;
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