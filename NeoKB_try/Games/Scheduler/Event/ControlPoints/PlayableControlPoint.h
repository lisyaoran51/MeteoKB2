#ifndef PLAYABLE_CONTROL_POINT_H
#define PLAYABLE_CONTROL_POINT_H


#include "HandType.h"
#include "MarkControlPoint.h"



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	/// <summary>
	/// 須輸入才會自動執行的control point
	///	</summary>
	class PlayableControlPoint : public MarkControlPoint {

	public:

		PlayableControlPoint(MTO_FLOAT s, MTO_FLOAT l);

		virtual int ApplyDefaultValues(SmDifficulty* sDifficulty);
		SmDifficulty* GetSmDifficulty();

		MTO_FLOAT GetVolume();
		int SetVolume(MTO_FLOAT v);

		HandType GetHandType();
		int SetHandType(HandType h);


	protected:

		SmDifficulty* smDifficulty = nullptr;

		MTO_FLOAT volume = -1;

		HandType handType = HandType::None;

	};





}}}}






#endif