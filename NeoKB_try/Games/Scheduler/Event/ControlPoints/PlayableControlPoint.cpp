#include "PlayableControlPoint.h"


using namespace Games::Schedulers::Events::ControlPoints;

PlayableControlPoint::PlayableControlPoint(MTO_FLOAT s, MTO_FLOAT l): ControlPoint(s, l)
{
}

int PlayableControlPoint::ApplyDefaultValues(SmDifficulty * sDifficulty)
{
	smDifficulty = sDifficulty;
	return 0;
}

SmDifficulty * PlayableControlPoint::GetSmDifficulty()
{
	return smDifficulty;
}



MTO_FLOAT PlayableControlPoint::GetVolume() {
	return volume;
}

int PlayableControlPoint::SetVolume(MTO_FLOAT v) {
	volume = v;
	return 0;
}

int PlayableControlPoint::GetSectionIndex() {
	return sectionIndex;
}

int PlayableControlPoint::SetSectionIndex(int sI) {
	sectionIndex = sI;
	return 0;
}

int PlayableControlPoint::GetPartIndex()
{
	return partIndex;
}

int PlayableControlPoint::SetPartIndex(int pIndex)
{
	partIndex = pIndex;
	return 0;
}

HandType PlayableControlPoint::GetHandType() {
	return handType;
}

int PlayableControlPoint::SetHandType(HandType h) {
	handType = h;
	return 0;
}