#include "TimeController.h"

#include "../../Scheduler/Event/ControlPoints/NoteControlPoint.h"




using namespace Games::Scenes::Play;
using namespace Framework::Timing::SpeedAdjusters;
using namespace Framework::Timing;
using namespace Games::Schedulers::Events::ControlPoints;



int TimeController::load()
{
	return 0;
}

int TimeController::update()
{
	if (isPaused)
		return 0;

	if (isTriggeredPause) {
		Pause();
		isTriggeredPause = false;
	}

	if (isTriggeredResume) {
		Resume();
		isTriggeredResume = false;
	}
	
	if (isTriggeredSeekingTime) {
		JumpTo(targetSeekTime);
		isTriggeredSeekingTime = false;
	}

	if (isSeekingTime) {
		JumpTo(targetSeekTime);
	}


	return 0;
}

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



TimeController::TimeController() : RegisterType("TimeController")
{
	registerLoad(bind(static_cast<int(TimeController::*)(void)>(&TimeController::load), this));

	isInputable = true;
	isPresent = true;
}

int TimeController::SetAudioClock(AdjustableClock * dInterpolatingFramedClock)
{
	
	audioClock = dInterpolatingFramedClock;
	return 0;
}

int TimeController::SetControllableClock(DecoupledInterpolatingFramedClock * cClock)
{
	controllableClock = cClock;
	return 0;
}

int TimeController::SetFramedClock(FramedClock * fClock)
{
	framedClock = fClock;
	return 0;
}

int TimeController::SetSpeedAdjuster(SpeedAdjuster * sAdjuster)
{
	// TODO: 做一個thread safe lock再刪
	delete speedAdjuster;
	speedAdjuster = sAdjuster;
	return 0;
}

int TimeController::JumpTo(double seekTime)
{
	audioClock->Seek(seekTime);
	controllableClock->Seek(seekTime);
	return 0;
}

int TimeController::JumpToWithSpeedAdjust(double seekTime)
{
	JumpTo(speedAdjuster->GenerateTime(GetClock()->GetElapsedFrameTime()));
	return 0;
}

int TimeController::Pause()
{
	audioClock->Stop();

	controllableClock->SetIsProcessSourceClockFrames(false);

	isPaused = true;

	return 0;
}

int TimeController::Resume()
{
	if (rate == 1) {
		audioClock->Seek(controllableClock->GetCurrentTime());
		audioClock->Start();
	}
	controllableClock->SetIsProcessSourceClockFrames(true);
	isPaused = false;
	return 0;
}

int TimeController::SetRate(double r)
{
	if (r == 0)
		throw invalid_argument("TimeController::SetRate() : 0 is not available for rate.");
	if (r != 1)
		audioClock->Stop();
	rate = r;
	audioClock->SetRate(r);
	controllableClock->SetRate(r);
	return 0;
}

double TimeController::GetRate()
{
	return rate;
}

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
