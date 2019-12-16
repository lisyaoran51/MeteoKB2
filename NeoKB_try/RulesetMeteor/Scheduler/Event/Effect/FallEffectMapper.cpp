#include "FallEffectMapper.h"

#include <math.h>
#include "../../../Ruleset/Judgements/MeteorJudgement.h"



using namespace Meteor::Schedulers::Events::Effects;
using namespace std;
using namespace Meteor::Rulesets::Judgements;


FallEffectMapper::FallEffectMapper(int w, int h) : EffectMapper<FallEffect>(w, h)
{

}

double FallEffectMapper::TryJudgement()
{
	// �����N���٨S�쩳�A�t���N��w�g�W�L�A���L�F
	double offset = GetStartTime() - GetCurrentTime();
	double absOffset = fabs(offset);

	FallEffect* fallEffect = dynamic_cast<FallEffect*>(GetEvent());

	if (absOffset > fallEffect->GetHitWindow(HitResult::Miss)) {
		if (offset > 0)
			return -1; // �N���٨S��
		else
			return -2; // �N��w�g�W�L�F�A�S���Q����

	}

	return offset + fallEffect->GetHitWindow(HitResult::Miss);
}

Pitch FallEffectMapper::GetPitch()
{
	return GetEffect()->GetPitch();
}

int FallEffectMapper::CheckForJudgement(bool isUserTriggered, double timeOffset)
{

	LOG(LogLevel::Debug) << "FallEffectMapper::CheckForJudgement() : checking if judgement is hit.";

	double offset = fabs(timeOffset);

	FallEffect* fallEffect = dynamic_cast<FallEffect*>(GetEvent());

	if (!isUserTriggered) {
		if (offset > fallEffect->GetHitWindow(HitResult::Miss)) {

			AddJudgement(new MeteorJudgement(fallEffect->GetResultFor(offset)));

		}
			
	}

	if (offset > fallEffect->GetHitWindow(HitResult::Miss))
		return -1;

	AddJudgement(new MeteorJudgement(fallEffect->GetResultFor(offset)));

	return 0;
}
