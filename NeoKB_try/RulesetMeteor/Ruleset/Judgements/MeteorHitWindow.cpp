#include "MeteorHitWindow.h"


using namespace Meteor::Rulesets::Judgements;



HitResult MeteorHitWindow::GetResultFor(double hitOffset)
{
	if (hitOffset < 0.08)
		return HitResult::Perfect;

	if (hitOffset < 0.2)
		return HitResult::Great;

	if (hitOffset < 0.3)
		return HitResult::Good;

	if (hitOffset < 0.44)
		return HitResult::Ok;

	if (hitOffset < 0.54)
		return HitResult::Bad;

	return HitResult::Miss;;
}

double MeteorHitWindow::GetHitWindow(HitResult hResult)
{
	switch (hResult) {
	case HitResult::None:
		return 100;
	case HitResult::Miss:
		return 0.7;
	case HitResult::Bad:
		return 0.54;
	case HitResult::Ok:
		return 0.44;
	case HitResult::Good:
		return 0.3;
	case HitResult::Great:
		return 0.2;
	case HitResult::Perfect:
		return 0.08;

	}
	return 100;
}
