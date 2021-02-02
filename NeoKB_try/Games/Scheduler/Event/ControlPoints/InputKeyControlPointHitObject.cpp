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
	// �����N���٨S�쩳�A�t���N��w�g�W�L�A���L�F
	double offset = GetStartTime() - GetCurrentTime();
	double absOffset = fabs(offset);

	LOG(LogLevel::Depricated) << "InputKeyControlPointHitObject::TryJudgement() : offset: " << offset << ", hit window: " << getHitWindow();

	if (absOffset > getHitWindow()->GetHitWindow(HitResult::Bad)) {
		if (offset > 0)
			return -1; // �N���٨S��
		else
			return -2; // �N��w�g�W�L�F�A�S���Q����

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
