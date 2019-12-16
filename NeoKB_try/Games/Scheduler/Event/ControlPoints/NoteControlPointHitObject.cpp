#include "NoteControlPointHitObject.h"



using namespace Games::Schedulers::Events::ControlPoints;



NoteControlPointHitObject::NoteControlPointHitObject(HitWindow * hWindow)
{
}

string NoteControlPointHitObject::GetTypeName()
{
	
	return "NoteControlPointHitObject";

}

double NoteControlPointHitObject::TryJudgement()
{
	// 正的代表還沒到底，負的代表已經超過，錯過了
	double offset = GetStartTime() - GetCurrentTime();
	double absOffset = fabs(offset);

	if (absOffset > getHitWindow()->GetHitWindow(HitResult::Bad)) {
		if (offset > 0)
			return -1; // 代表還沒到
		else
			return -2; // 代表已經超過了，沒有被按到

	}

	return offset + getHitWindow()->GetHitWindow(HitResult::Bad);
}

NoteControlPoint * NoteControlPointHitObject::GetNoteControlPoint()
{
	return dynamic_cast<NoteControlPoint*>(GetEvent());
}

int NoteControlPointHitObject::CheckForJudgement(bool isUserTriggered, double timeOffset)
{
	LOG(LogLevel::Depricated) << "NoteControlPointHitObject::CheckForJudgement() : checking if judgement is hit.";

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
