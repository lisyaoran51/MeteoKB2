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
	// 正的代表還沒到底，負的代表已經超過，錯過了
	double offset = GetStartTime() - GetCurrentTime();
	double absOffset = fabs(offset);

	FallEffect* fallEffect = dynamic_cast<FallEffect*>(GetEvent());

	if (absOffset > fallEffect->GetHitWindow(HitResult::Miss)) {
		if (offset > 0)
			return -1; // 代表還沒到
		else
			return -2; // 代表已經超過了，沒有被按到

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
