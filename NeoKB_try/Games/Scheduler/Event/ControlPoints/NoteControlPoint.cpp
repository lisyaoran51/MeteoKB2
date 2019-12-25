#include "NoteControlPoint.h"

using namespace Games::Schedulers::Events::ControlPoints;


NoteControlPoint::NoteControlPoint(Pitch p, MTO_FLOAT s, MTO_FLOAT l): PlayableControlPoint(s,l), HasPitch(p)
{
	pitch = p;
}

ControlPointType NoteControlPoint::GetControlPointType()
{
	return ControlPointType::Note;
}

string NoteControlPoint::GetTypeName()
{
	return "NoteControlPoint";
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
	newNoteControlPoint->SetPartIndex(partIndex);
	return newNoteControlPoint;
}
