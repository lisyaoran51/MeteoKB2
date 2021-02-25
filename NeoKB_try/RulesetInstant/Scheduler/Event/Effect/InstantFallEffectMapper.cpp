#include "InstantFallEffectMapper.h"




using namespace Instant::Schedulers::Events::Effects;


InstantFallEffectMapper::InstantFallEffectMapper()
{
	throw logic_error("InstantFallEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
}

InstantFallEffectMapper::InstantFallEffectMapper(int w, int h) : EffectMapper<InstantFallEffect>(w, h)
{

}


/* ����MeteorNoteControlPointHitObject�F
double FallEffectMapper::TryJudgement()
{
	// �����N���٨S�쩳�A�t���N��w�g�W�L�A���L�F
	double offset = GetStartTime() - GetCurrentTime();
	double absOffset = fabs(offset);

	FallEffect* fallEffect = dynamic_cast<FallEffect*>(GetEvent());

	LOG(LogLevel::Debug) << "FallEffectMapper::TryJudgement() : offset = " << offset;

	if (absOffset > fallEffect->GetHitWindow(HitResult::Bad)) {
		if (offset > 0)
			return -1; // �N���٨S��
		else
			return -2; // �N��w�g�W�L�F�A�S���Q����

	}

	return offset + fallEffect->GetHitWindow(HitResult::Bad);
}

Pitch FallEffectMapper::GetPitch()
{
	return GetEffect()->GetPitch();
}

int FallEffectMapper::CheckForJudgement(bool isUserTriggered, double timeOffset)
{

	LOG(LogLevel::Depricated) << "FallEffectMapper::CheckForJudgement() : checking if judgement is hit.";

	double offset = fabs(timeOffset);

	FallEffect* fallEffect = dynamic_cast<FallEffect*>(GetEvent());

	if (!isUserTriggered) {
		if (offset > fallEffect->GetHitWindow(HitResult::Miss)) {

			AddJudgement(new MeteorJudgement(fallEffect->GetResultFor(offset)));

		}
			
	}

	if (offset > fallEffect->GetHitWindow(HitResult::Bad))
		return -1;

	AddJudgement(new MeteorJudgement(fallEffect->GetResultFor(offset)));

	return 0;
}
*/