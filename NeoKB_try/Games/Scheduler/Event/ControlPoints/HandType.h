#ifndef HAND_TYPE_H
#define HAND_TYPE_H





namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	enum class HandType {

		/* 單一譜檔 */
		None,
		LeftEasy,				// 1 左手簡單 如果沒有分手右手，就一律左手，玫芬難度的話就一律簡單
		LeftOther,				// 2 左手簡單以外的音
		RightEasy,				// 3 右手簡單
		RightOther,				// 4 右手簡單以外的音
		Foot,					// 5 踏板
		OctaveShift,			// 6 移調
		Hidden,					// 7 不分左右手、難度的隱藏燈光音
		All,					// 8 任何都可以

		/* 複合式譜檔 */
		LeftOnly,					// 9 純左手
		LeftOnlyFoot,				// 10 純左手 踏板
		LeftOnlyOctaveShift,		// 11 純左手 移調
		HiddenLeftOnly,				// 12 純左手 隱藏燈光音 

		RightOnly,					// 13 純右手
		RightOnlyFoot,				// 14 純右手 踏板 
		RightOnlyOctaveShift,		// 15 純右手 移調
		HiddenRightOnly,			// 16 純右手 隱藏燈光音 

		EasyOnly,					// 17 純簡單
		EasyOnlyFoot,				// 18 純簡單 踏板 
		EasyOnlyOctaveShift,		// 19 純簡單 移調
		HiddenEasyOnly,				// 20 純簡單 隱藏燈光音 

		HardOnly,					// 21 純困難
		HardOnlyFoot,				// 22 純困難 踏板 
		HardOnlyOctaveShift,		// 23 純困難 移調
		HiddenHardOnly,				// 24 純困難 隱藏燈光音 

		LeftEasyOnly,				// 25 純左手簡單
		LeftEasyOnlyFoot,			// 26 純左手簡單 踏板 
		LeftEasyOnlyOctaveShift,	// 27 純左手簡單 移調
		HiddenLeftEasyOnly,			// 28 純左手簡單 隱藏燈光音 

		LeftHardOnly,				// 29 純左手困難
		LeftHardOnlyFoot,			// 30 純左手困難 踏板
		LeftHardOnlyOctaveShift,	// 31 純左手困難 移調
		HiddenLeftHardOnly,			// 32 純左手困難 隱藏燈光音 

		RightEasyOnly,				// 33 純右手簡單
		RightEasyOnlyFoot,			// 34 純右手簡單 踏板
		RightEasyOnlyOctaveShift,	// 35 純右手簡單 移調
		HiddenRightEasyOnly,		// 36 純右手簡單 隱藏燈光音 

		RightHardOnly,				// 37 純右手困難
		RightHardOnlyFoot,			// 38 純右手困難 踏板
		RightHardOnlyOctaveShift,	// 39 純右手困難 移調
		HiddenRightHardOnly,		// 40 純右手困難 隱藏燈光音 
		//Auto,	// 這個是不是舊的要刪掉? 6通常是平移八度
	};



}}}}


#endif