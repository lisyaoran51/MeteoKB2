#include "MeteorHitWindow.h"


using namespace Meteor::Rulesets::Judgements;



HitResult MeteorHitWindow::GetResultFor(double hitOffset)
{
	if (hitOffset < 0.04)
		return HitResult::Perfect;

	if (hitOffset < 0.1)
		return HitResult::Great;

	if (hitOffset < 0.15)
		return HitResult::Good;

	if (hitOffset < 0.22)
		return HitResult::Ok;

	if (hitOffset < 0.27)
		return HitResult::Meh;

	return HitResult::Miss;;
}

double MeteorHitWindow::GetHitWindow(HitResult hResult)
{
	switch (hResult) {
	case HitResult::None:
		return 100;
	case HitResult::Miss:
		return 0.35;
	case HitResult::Meh:
		return 0.27;
	case HitResult::Ok:
		return 0.22;
	case HitResult::Good:
		return 0.15;
	case HitResult::Great:
		return 0.1;
	case HitResult::Perfect:
		return 0.04;

	}
	return 100;
}
