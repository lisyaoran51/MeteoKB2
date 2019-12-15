#include "HasHitWindow.h"



using namespace Games::Rulesets::Judgements;

HitResult HasHitWindow::GetResultFor(double offset)
{
	return hitWindow->GetResultFor(offset);
}

double HasHitWindow::GetHitWindow(HitResult window)
{
	return hitWindow->GetHitWindow(window);
}
