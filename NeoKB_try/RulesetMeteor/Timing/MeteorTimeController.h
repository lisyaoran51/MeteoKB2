#ifndef METEOR_TIME_CONTROLLER_H
#define METEOR_TIME_CONTROLLER_H


#include "../../Games/Timing/MeteoTimeController.h"
#include "../Input/MeteorInputManager.h"


using namespace Meteor::Input;
using namespace Framework::Timing;
using namespace Games::Timing;


namespace Meteor {
namespace Timing {

	/// <summary>
	/// 遊戲類別應該要擺在別的地方，但暫時想步道要擺哪，先擺這邊一下
	/// </summary>
	enum class MeteorTimeControllerMode {
		None,
		MusicGame,
		RepeatPractice
	};

	enum class RepeatPracticeMode {
		None,
		Demonstrate,
		Practice
	};

	class MeteorTimeController : public MeteoTimeController<MeteorAction> {

		int load();

		int loadOnComplete(EventProcessorFilter* e);

		/// <summary>
		/// 再反覆練彈模式中，避免彈到別的段落的音，所使用的filter
		/// </summary>
		bool filterEventBySection(EventProcessor<Event>* eventProcessor);

		/// <summary>
		/// 反覆練彈模視中，用來示範的
		/// </summary>
		bool filterEruptEffect(EventProcessor<Event>* eventProcessor);

		/// <summary>
		/// 反覆練彈模視中，用來練習的
		/// </summary>
		bool filterFallEffect(EventProcessor<Event>* eventProcessor);

	public:

		MeteorTimeController();

		virtual int SetRate(double r);

		/* ------------------------ TimeController.h ------------------------ */

		int SetLastEventOverTime(double lEventOverTime);

		virtual int OnButtonDown(MeteorAction action);

		virtual int OnKnobTurn(pair<MeteorAction, int> action);


		/*---------反覆練彈專用---------*/
		/// <summary>
		/// 要拿到每個小節的秒數，之後再轉小節旋鈕時才能轉到對的位置。如果沒有給小節秒數的話，小節旋鈕就統一跳3秒
		/// 會再meteor ruleset executor裡面設定
		/// </summary>
		int SetSectionTime(vector<float>* sTime);

		int SetTimeControllerMode(MeteorTimeControllerMode tControllerMode);

		int SetRepeatSections(int rSections);

		int SetRepeatTimes(int rTimes);

		int SetHasSection(bool hSection);

		/// <summary>
		/// 反覆練彈的反覆功能
		/// </summary>
		int RepeatSection(int section);
		/*---------反覆練彈專用---------*/


	protected:




		double lastEventOverTime = -1;

		MeteorTimeControllerMode timeControllerMode = MeteorTimeControllerMode::MusicGame;

		/// <summary>
		/// 在update時檢查有沒有被暫停，有的話就把自己時鐘停掉
		/// </summary>
		virtual int update();

		virtual int LoadOnComplete();

		/// <summary>
		/// 檢查遊戲是否結束，可以根據遊戲event是否全部跑完來判斷，也可以根據不同規則判斷
		/// </summary>
		virtual bool checkIsGameOver();

		/*---------反覆練彈專用---------*/ // 這個不適應該擺再repeat practice event processor裡面嗎?
		bool hasSection = false;

		/// <summary>
		/// 反覆練彈的開始小節
		/// </summary>
		int tempRepeatStartSection = 0;

		/// <summary>
		/// 每一次反覆練彈內含幾小節
		/// </summary>
		int maxSectionAmountForOneRepeat = 1;

		/// <summary>
		/// 目前彈到幾小節
		/// </summary>
		//int s = 0;

		/// <summary>
		/// 每個循環要反覆練彈幾次
		/// </summary>
		int maxRepeatCounts = 4;

		/// <summary>
		/// 目前這個循環中反覆談了幾次
		/// </summary>
		int tempRepeatCounts = 0;

		/// <summary>
		/// 目前是在一次中的示範還是練習
		/// </summary>
		RepeatPracticeMode repeatPracticeMode = RepeatPracticeMode::None;

		/// <summary>
		/// 所有小節開始秒數
		/// </summary>
		vector<float> sectionTime;

		float repeatBufferTime = 1;
		/*---------反覆練彈專用---------*/

		EventProcessorFilter* eventProcessorFilter = nullptr;

		virtual int onMessage(MeteoBluetoothMessage* message);

	};


}
}





#endif