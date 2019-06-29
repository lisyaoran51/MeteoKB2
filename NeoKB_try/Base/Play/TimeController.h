#ifndef TIME_CONTROLLER_H
#define TIME_CONTROLLER_H

namespace Game {
namespace Scene {
namespace Play {

	/// <summary>
	/// 擺在player下面，用來控制遊戲速度和暫停、跳小節
	///	</summary>
	/// <summary>
	/// osu本身沒有暫停鍵，暫停就要縮小視窗，所以沒有人會去控制pause container，而是pause container自己去詢問host是否active，如果
	/// 視窗縮小才會pause
	/// 
	/// 控制的方法應該是 再playfield load的時候，把Event processor的algorithm add child，讓這個algorithm有pause container的資料
	/// 然後在playfield add event processor的時候，再把algorithm擺入這些processor裡。
	///
	/// pause container的時鐘是在player裡面指派的，不是pause container自己的
	/// </summary>
	class TimeController : public Scene {
	public:

		/// <summary>
		/// pause container的時鐘是在player裡面指派的，不是pause container自己的
		/// </summary>
		int SetFramedClock(FramedClock* fClock);

		int SetAdjustableClock(AdjustableClock* aClock);


	protected:


	private:

		int load();

		/// <summary>
		/// 在update時檢查有沒有被暫停，有的話就把自己時鐘停掉
		/// </summary>
		int update();

		/// <summary>
		/// 一個與parent獨立的時終，下面接的式遊戲的物件，遊戲根據這個時鐘運行
		/// </summary>
		FramedClock* framedClock;

		AdjustableClock* audioClock;
	};




}}}







#endif