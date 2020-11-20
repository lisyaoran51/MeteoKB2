#ifndef HAND_TYPE_H
#define HAND_TYPE_H





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
		//Auto,	// 這個是不是舊的要刪掉?
		OctaveShift,
		HiddenNone,
		HiddenLeft,
		HiddenRight,
	};



}}}}


#endif