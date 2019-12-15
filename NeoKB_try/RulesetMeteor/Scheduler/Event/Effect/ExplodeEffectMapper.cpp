#include "ExplodeEffectMapper.h"



using namespace Meteor::Schedulers::Events::Effects;
using namespace std;


ExplodeEffectMapper::ExplodeEffectMapper(int w, int h, HitObject* hObject) : EffectMapper<ExplodeEffect>(w, h)
{
	hitObject = hObject;
	explodeStartTime = hObject->GetCurrentTime();
	lifeType = EventProcessorLifeType::Timed;
}

MTO_FLOAT ExplodeEffectMapper::GetStartTime()
{
	return explodeStartTime;
}

double ExplodeEffectMapper::GetCurrentTime()
{
	return hitObject->GetCurrentTime();
}

int ExplodeEffectMapper::SetLifeTime(MTO_FLOAT lTime)
{
	lifeTime = lTime;
	return 0;
}

MTO_FLOAT ExplodeEffectMapper::GetTimeLeft()
{
	return lifeTime - (GetCurrentTime() - GetStartTime());
}

MTO_FLOAT ExplodeEffectMapper::GetLifeTime()
{
	return lifeTime;
}
