#ifndef HAND_TYPE_H
#define HAND_TYPE_H





namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	enum class HandType {
		None,
		LeftEasy,	// 如果沒有分手右手，就一律左手，玫芬難度的話就一律簡單
		LeftOther,
		RightEasy,
		RightOther,
		Foot,
		OctaveShift, 
		HiddenLeft,
		HiddenRight,
		Hidden,	// 不分左右手
		//Auto,	// 這個是不是舊的要刪掉? 6通常是平移八度
	};



}}}}


#endif