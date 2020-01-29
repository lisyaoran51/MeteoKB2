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

string Judgement::GetResultString()
{
	switch (hitResult) {
	case HitResult::Miss:
		return "Miss";
	case HitResult::Bad:
		return "Bad";
	case HitResult::Ok:
		return "Ok";
	case HitResult::Good:
		return "Good";
	case HitResult::Great:
		return "Great";
	case HitResult::Perfect:
		return "Perfect";
	}

	return "";
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
