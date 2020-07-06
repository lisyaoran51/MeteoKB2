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

	class MeteorTimeController : public TimeController {

		int load();

		int load(EventProcessorFilter* eProcessorFilter);

	public:

		MeteorTimeController();

		/// <summary>
		/// 要拿到每個小節的秒數，之後再轉小節旋鈕時才能轉到對的位置。如果沒有給小節秒數的話，小節旋鈕就統一跳3秒
		/// 會再meteor ruleset executor裡面設定
		/// </summary>
		int SetSectionTime(vector<float>* sTime);

		int SetTimeControllerMode(MeteorTimeControllerMode tControllerMode);

		int SetRepeatSections(int rSections);

		int SetHasSection(bool hSection);

		/// <summary>
		/// TODO:
		/// 拿到action和input的map，這個應該要擺在Meteor Ruleset才對，之後要移過去，然後中間家一層Ruleset Time Controller
		/// 再裡面存ruleset info，才能直接從time controller裡面找出ruleset的keybinding
		/// 現在懶得改，先擺在自己裡面就好。以後要修改action和input的mapping都要記得進來這邊修
		/// </summary>
		//virtual map<MeteorAction, InputKey>* GetDefaultkeyBindings(); // 改成不繼承meteo action


	protected:

		MeteorTimeControllerMode timeControllerMode = MeteorTimeControllerMode::MusicGame;

		bool hasSection = false;

		int repeatSections = -1;

		vector<float> sectionTime;

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