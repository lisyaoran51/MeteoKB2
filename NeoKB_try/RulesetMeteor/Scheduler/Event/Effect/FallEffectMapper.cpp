#include "FallEffectMapper.h"

#include <math.h>



using namespace Meteor::Schedulers::Events::Effects;
using namespace std;


FallEffectMapper::FallEffectMapper(int w, int h) : EffectMapper<FallEffect>(w, h)
{

}

int FallEffectMapper::CheckForJudgement(bool isUserTriggered, double timeOffset)
{

	double offset = fabs(timeOffset);

	FallEffect* fallEffect = dynamic_cast<FallEffect*>(GetEvent());

	if (offset > fallEffect->GetHitWindow(HitResult::Miss))
		return -1;

	AddJudgement(new MeteorJudgement(fallEffect->GetResultFor(offset)));

	return 0;
}
