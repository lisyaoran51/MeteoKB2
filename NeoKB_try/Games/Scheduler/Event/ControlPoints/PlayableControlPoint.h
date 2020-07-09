#ifndef PLAYABLE_CONTROL_POINT_H
#define PLAYABLE_CONTROL_POINT_H


#include "HandType.h"
#include "ControlPoint.h"



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{


	class PlayableControlPoint : public ControlPoint {

	public:

		PlayableControlPoint(MTO_FLOAT s, MTO_FLOAT l);

		virtual int ApplyDefaultValues(SmDifficulty* sDifficulty);
		SmDifficulty* GetSmDifficulty();

		MTO_FLOAT GetVolume();
		int SetVolume(MTO_FLOAT v);

		int GetSectionIndex();
		int SetSectionIndex(int sI);

		int GetPartIndex();
		int SetPartIndex(int pIndex);

		HandType GetHandType();
		int SetHandType(HandType h);


	protected:

		SmDifficulty* smDifficulty = nullptr;

		MTO_FLOAT volume = -1;

		int sectionIndex = -1;
		int partIndex = -1;

		HandType handType = HandType::None;

	};





}}}}






#endif