#ifndef PLAYABLE_CONTROL_POINT_H
#define PLAYABLE_CONTROL_POINT_H


#include "ControlPoint.h"



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	enum class HandType {
		None,
		LeftEasy,
		LeftOther,
		RightEasy,
		RightOther,
		Foot,
		HiddenNone,
		HiddenLeft,
		HiddenRight
	};

	class PlayableControlPoint : public ControlPoint {

	public:

		PlayableControlPoint(MTO_FLOAT s, MTO_FLOAT l);

		MTO_FLOAT GetVolume();
		int SetVolume(MTO_FLOAT v);

		int GetSectionIndex();
		int SetSectionIndex(int sI);

		int GetPartIndex();
		int SetPartIndex(int pIndex);

		HandType GetHandType();
		int SetHandType(HandType h);


	protected:

		MTO_FLOAT volume = -1;

		int sectionIndex = -1;
		int partIndex = -1;

		HandType handType = HandType::None;

	};





}}}}






#endif