#include "Score.h"


using namespace Games::Rulesets::Scoring;


Score::Score()
{
	hitResults[HitResult::None] = 0;
	hitResults[HitResult::Miss] = 0;
	hitResults[HitResult::Bad] = 0;
	hitResults[HitResult::Ok] = 0;
	hitResults[HitResult::Good] = 0;
	hitResults[HitResult::Great] = 0;
	hitResults[HitResult::Perfect] = 0;
}
