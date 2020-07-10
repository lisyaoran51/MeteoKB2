#ifndef METEOR_TIME_CONTROLLER_H
#define METEOR_TIME_CONTROLLER_H

#include "../../Framework/Timing/TimeController.h"
#include "../Input/MeteorInputManager.h"


using namespace Meteor::Input;
using namespace Framework::Timing;


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

	class MeteorTimeController : public TimeController {

		int load();

		int loadOnComplete(EventProcessorFilter* e);

		/// <summary>
		/// 再反覆練彈模式中，避免彈到別的段落的音，所使用的filter
		/// </summary>
		int filterEventBySection(vector<EventProcessor<Event>*>* eventProcessors);

		/// <summary>
		/// 反覆練彈模視中，用來示範的
		/// </summary>
		int filterEruptEffect(vector<EventProcessor<Event>*>* eventProcessors);

		/// <summary>
		/// 反覆練彈模視中，用來練習的
		/// </summary>
		int filterFallEffect(vector<EventProcessor<Event>*>* eventProcessors);

	public:

		MeteorTimeController();

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

		virtual int LoadOnComplete();

		MeteorTimeControllerMode timeControllerMode = MeteorTimeControllerMode::MusicGame;

		/*---------反覆練彈專用---------*/
		bool hasSection = false;

		/// <summary>
		/// 反覆練彈的開始小節
		/// </summary>
		int tempStartSection = 0;

		/// <summary>
		/// 反覆練彈依次幾小節
		/// </summary>
		int repeatSections = 1;

		/// <summary>
		/// 目前彈到幾小節
		/// </summary>
		int tempSection = 0;

		/// <summary>
		/// 反覆練彈幾次
		/// </summary>
		int repeatTimes = 4;

		/// <summary>
		/// 目前反覆談了幾次
		/// </summary>
		int tempRepeatTimes = 0;

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

		/// <summary>
		/// 把input key和新的輸入結合一下
		/// </summary>
		//virtual int reloadMappings(); // 改成不繼承meteo action

		virtual int onButtonDown(InputState* inputState, InputKey button);

		virtual int onKnobTurn(InputState* inputState, InputKey knob);

	};


}
}





#endif