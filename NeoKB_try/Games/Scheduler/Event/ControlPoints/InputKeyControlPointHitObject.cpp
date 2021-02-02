#include "InputKeyControlPointHitObject.h"

using namespace Games::Schedulers::Events::ControlPoints;



InputKeyControlPointHitObject::InputKeyControlPointHitObject(HitWindow * hWindow): HitObject(hWindow), HasInputKey(InputKey::None, 0)
{
	eventProcessorType = EventProcessorType::ControlPoint;
}

string InputKeyControlPointHitObject::GetTypeName()
{
	return "InputKeyControlPointHitObject";
}

double InputKeyControlPointHitObject::TryJudgement()
{
	// 正的代表還沒到底，負的代表已經超過，錯過了
	double offset = GetStartTime() - GetCurrentTime();
	double absOffset = fabs(offset);

	LOG(LogLevel::Depricated) << "InputKeyControlPointHitObject::TryJudgement() : offset: " << offset << ", hit window: " << getHitWindow();

	if (absOffset > getHitWindow()->GetHitWindow(HitResult::Bad)) {
		if (offset > 0)
			return -1; // 代表還沒到
		else
			return -2; // 代表已經超過了，沒有被按到

	}

	return offset + getHitWindow()->GetHitWindow(HitResult::Bad);
}

InputKeyControlPointHitObject * InputKeyControlPointHitObject::GetInputKeyControlPoint()
{
	return dynamic_cast<InputKeyControlPointHitObject*>(GetEvent());
}

InputKey InputKeyControlPointHitObject::GetInputKey()
{
	return dynamic_cast<InputKeyControlPointHitObject*>(GetEvent())->GetInputKey();
}

int InputKeyControlPointHitObject::GetInputValue()
{
	return dynamic_cast<InputKeyControlPointHitObject*>(GetEvent())->GetInputValue();
}

int InputKeyControlPointHitObject::CheckForJudgement(bool isUserTriggered, double timeOffset)
{
	LOG(LogLevel::Depricated) << "InputKeyControlPointHitObject::CheckForJudgement() : checking if judgement is hit.";

	double offset = fabs(timeOffset);

	if (!isUserTriggered) {
		if (offset > getHitWindow()->GetHitWindow(HitResult::Bad)) {

			AddJudgement(createJudgement(getHitWindow()->GetResultFor(offset)));

		}

	}

	if (offset > getHitWindow()->GetHitWindow(HitResult::Bad))
		return -1;

	AddJudgement(createJudgement(getHitWindow()->GetResultFor(offset)));

	return 0;
}
