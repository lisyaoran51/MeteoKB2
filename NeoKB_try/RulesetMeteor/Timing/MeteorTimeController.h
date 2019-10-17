#ifndef METEOR_TIME_CONTROLLER_H
#define METEOR_TIME_CONTROLLER_H

#include "../../Games/Scene/Play/TimeController.h"
#include "../../Instruments/Input/PianoAction.h"


using namespace Games::Scenes::Play;
using namespace Instruments::Input;


namespace Meteor {
namespace Timing {

	class MeteorTimeController : public TTimeController<PianoAction> {


	public:

		MeteorTimeController();

		/// <summary>
		/// TODO:
		/// 拿到action和input的map，這個應該要擺在Meteor Ruleset才對，之後要移過去，然後中間家一層Ruleset Time Controller
		/// 再裡面存ruleset info，才能直接從time controller裡面找出ruleset的keybinding
		/// 現在懶得改，先擺在自己裡面就好。以後要修改action和input的mapping都要記得進來這邊修
		/// </summary>
		virtual map<PianoAction, InputKey>* GetDefaultkeyBindings();


	protected:


		/// <summary>
		/// 把input key和新的輸入結合一下
		/// </summary>
		virtual int reloadMappings();

	};


}
}





#endif