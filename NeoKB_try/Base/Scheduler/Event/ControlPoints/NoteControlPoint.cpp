#include "NoteControlPoint.h"

using namespace Base::Schedulers::Events::ControlPoints;


NoteControlPoint::NoteControlPoint(Pitch p, MTO_FLOAT s, MTO_FLOAT l): ControlPoint(s,l)
{
	pitch = p;
}

ControlPointType NoteControlPoint::GetControlPointType()
{
	return ControlPointType::ControlPointTypeNote;
}

string NoteControlPoint::GetTypeName()
{
	return "NoteControlPoint";
}

Pitch NoteControlPoint::GetPitch()
{
	return pitch;
}

MTO_FLOAT NoteControlPoint::GetVolume(){
	return volume;
}

int NoteControlPoint::SetVolume(MTO_FLOAT v){
	volume = v;
	return 0;
}

int NoteControlPoint::GetSectionIndex(){
	return sectionIndex;
}

int NoteControlPoint::SetSectionIndex(int sI){
	sectionIndex = sI;
	return 0;
}

HandType NoteControlPoint::GetHandType(){
	return handType;
}

int NoteControlPoint::SetHandType(HandType h){
	handType = h;
	return 0;
}

bool NoteControlPoint::IsWhiteKey()
{
	switch (int(pitch) % 12) {
	case 0:
	case 2:
	case 4:
	case 5:
	case 7:
	case 9:
	case 11:
		return true;
	case 1:
	case 3:
	case 6:
	case 8:
	case 10:
		return false;
	}
	return false;
}

Event* NoteControlPoint::Clone()
{
	NoteControlPoint* newNoteControlPoint = new NoteControlPoint(pitch, startTime, lifeTime);
	newNoteControlPoint->SetVolume(volume);
	newNoteControlPoint->SetSectionIndex(sectionIndex);
	newNoteControlPoint->SetHandType(handType);
	return newNoteControlPoint;
}
