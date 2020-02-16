#ifndef AUTO_CONTROL_POINT_H
#define AUTO_CONTROL_POINT_H


#include "HandType.h"
#include "ControlPoint.h"



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{


	class AutoControlPoint : public ControlPoint {

	public:

		AutoControlPoint(MTO_FLOAT s, MTO_FLOAT l);

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

		HandType handType = HandType::Auto;

	};





}}}}






#endif