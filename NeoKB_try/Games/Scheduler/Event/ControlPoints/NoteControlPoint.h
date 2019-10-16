#ifndef NOTE_CONTROL_POINT_H
#define NOTE_CONTROL_POINT_H

#include "ControlPoint.h"
#include "../../../../Instruments/Pitch.h"

using namespace Instruments;


namespace Games {
namespace Schedulers {
namespace Events {
namespace ControlPoints {

	enum class HandType;

	class NoteControlPoint : public ControlPoint {

		Pitch pitch;
		
		MTO_FLOAT volume;
		
		int sectionIndex;
		int partIndex;
		
		HandType handType;

	public:

		NoteControlPoint(Pitch p, MTO_FLOAT s, MTO_FLOAT l);

		virtual ControlPointType GetControlPointType();

		// 一定要每次都override!!
		virtual string GetTypeName();

		Pitch GetPitch();
		
		MTO_FLOAT GetVolume();
		int SetVolume(MTO_FLOAT v);
		
		int GetSectionIndex();
		int SetSectionIndex(int sI);

		int GetPartIndex();
		int SetPartTndex(int pIndex);
		
		HandType GetHandType();
		int SetHandType(HandType h);

		bool IsWhiteKey();

		virtual Event* Clone();

	};

	
	enum class HandType{
		None,
		Left,
		Right,
		HiddenNone,
		HiddenLeft,
		HiddenRight
	};
	
}}}}





#endif