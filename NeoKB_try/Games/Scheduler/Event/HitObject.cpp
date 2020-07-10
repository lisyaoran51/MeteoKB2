#include "HitObject.h"


using namespace Games::Schedulers::Events;


int HitObject::setState(ArmedState s)
{
	// updateState(s);

	state = s;

	// play sample

	return 0;
}

HitObject::HitObject(HitWindow * hWindow)
{
	hitWindow = hWindow;
}

EventProcessor<Event>* HitObject::RegisterEvent(Event * e)
{
	if (dynamic_cast<PlayableControlPoint*>(e)) {
		if (dynamic_cast<PlayableControlPoint*>(e)->GetSmDifficulty()) {

			delete hitWindow;
			hitWindow = createHitWindow(dynamic_cast<PlayableControlPoint*>(e)->GetSmDifficulty());
			return this;
		}
	}

	LOG(LogLevel::Error) << "HitObject::RegisterEvent() : No Difficulty found at event [" << GetStartTime() << "].";
	return nullptr;
}

bool HitObject::GetHasJudgementResult()
{
	return hasJudgementResult;
}

bool HitObject::UpdateJudgement(bool isUserTriggered)
{

	CheckForJudgement(isUserTriggered, GetCurrentTime() - GetStartTime());

	return false;
}

HitWindow * HitObject::getHitWindow()
{
	return hitWindow;
}

int HitObject::AddJudgement(Judgement* judgement)
{
	LOG(LogLevel::Depricated) << "HitObject::AddJudgement() : add hit judgement.";

	hasJudgementResult = judgement->GetResult() >= HitResult::Miss;

	judgementOccurred = true;

	judgement->SetTimeOffset(GetCurrentTime() - GetStartTime());

	judgements.push_back(judgement);

	switch (judgement->GetResult())
	{
	case HitResult::None:
		break;
	case HitResult::Miss:
		setState(ArmedState::Miss);
		break;
	default:
		setState(ArmedState::Hit);
		break;
	}

	onJudgement.Trigger(this, judgement);

	return 0;
}
