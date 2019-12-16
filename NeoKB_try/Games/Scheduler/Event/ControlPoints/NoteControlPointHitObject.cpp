#include "NoteControlPointHitObject.h"



using namespace Games::Schedulers::Events::ControlPoints;



NoteControlPointHitObject::NoteControlPointHitObject(HitWindow * hWindow): HasPitch(Pitch::None), HitObject(hWindow)
{
	
}

string NoteControlPointHitObject::GetTypeName()
{
	
	return "NoteControlPointHitObject";

}

double NoteControlPointHitObject::TryJudgement()
{
	// �����N���٨S�쩳�A�t���N��w�g�W�L�A���L�F
	double offset = GetStartTime() - GetCurrentTime();
	double absOffset = fabs(offset);

	LOG(LogLevel::Depricated) << "NoteControlPointHitObject::TryJudgement() : offset: " << offset << ", hit window: " << getHitWindow();

	if (absOffset > getHitWindow()->GetHitWindow(HitResult::Bad)) {
		if (offset > 0)
			return -1; // �N���٨S��
		else
			return -2; // �N��w�g�W�L�F�A�S���Q����

	}

	return offset + getHitWindow()->GetHitWindow(HitResult::Bad);
}

NoteControlPoint * NoteControlPointHitObject::GetNoteControlPoint()
{
	return dynamic_cast<NoteControlPoint*>(GetEvent());
}

Pitch NoteControlPointHitObject::GetPitch()
{
	return dynamic_cast<HasPitch*>(event)->GetPitch();
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
