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
		LeftOnly = 11,				// 11 純左手
		LeftOnlyFoot,				// 12 純左手 踏板
		LeftOnlyOctaveShift,		// 13 純左手 移調
		LeftOnlyHidden,				// 14 純左手 隱藏燈光音 

		RightOnly,					// 15 純右手
		RightOnlyFoot,				// 16 純右手 踏板 
		RightOnlyOctaveShift,		// 17 純右手 移調
		RightOnlyHidden,			// 18 純右手 隱藏燈光音 

		EasyOnly,					// 19 純簡單
		EasyOnlyFoot,				// 20 純簡單 踏板 
		EasyOnlyOctaveShift,		// 21 純簡單 移調
		EasyOnlyHidden,				// 22 純簡單 隱藏燈光音 

		HardOnly,					// 23 純困難
		HardOnlyFoot,				// 24 純困難 踏板 
		HardOnlyOctaveShift,		// 25 純困難 移調
		HardOnlyHidden,				// 26 純困難 隱藏燈光音 

		LeftEasyOnly,				// 27 純左手簡單
		LeftEasyOnlyFoot,			// 28 純左手簡單 踏板 
		LeftEasyOnlyOctaveShift,	// 29 純左手簡單 移調
		LeftEasyOnlyHidden,			// 30 純左手簡單 隱藏燈光音 

		LeftHardOnly,				// 31 純左手困難
		LeftHardOnlyFoot,			// 32 純左手困難 踏板
		LeftHardOnlyOctaveShift,	// 33 純左手困難 移調
		LeftHardOnlyHidden,			// 34 純左手困難 隱藏燈光音 

		RightEasyOnly,				// 35 純右手簡單
		RightEasyOnlyFoot,			// 36 純右手簡單 踏板
		RightEasyOnlyOctaveShift,	// 37 純右手簡單 移調
		RightEasyOnlyHidden,		// 38 純右手簡單 隱藏燈光音 

		RightHardOnly,				// 39 純右手困難
		RightHardOnlyFoot,			// 40 純右手困難 踏板
		RightHardOnlyOctaveShift,	// 41 純右手困難 移調
		RightHardOnlyHidden,		// 42 純右手困難 隱藏燈光音 
		//Auto,	// 這個是不是舊的要刪掉? 6通常是平移八度
	};



}}}}


#endif