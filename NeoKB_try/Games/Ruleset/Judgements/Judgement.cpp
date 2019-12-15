#include "Judgement.h"


using namespace Games::Rulesets::Judgements;


Judgement::Judgement(HitResult r)
{
	hitResult = r;
}

HitResult Judgement::GetResult()
{
	return hitResult;
}

double Judgement::GetResultScore()
{
	return getResultNumber(hitResult);
}

double Judgement::GetMaxResultScore()
{
	return getResultNumber(HitResult::Perfect);
}

int Judgement::SetTimeOffset(double offset)
{
	timeOffset = offset;
	return 0;
}

bool Judgement::GetIsHit()
{
	return hitResult > HitResult::Miss;
}
