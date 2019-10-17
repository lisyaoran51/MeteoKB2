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
		/// ����action�Minput��map�A�o�����ӭn�\�bMeteor Ruleset�~��A����n���L�h�A�M�ᤤ���a�@�hRuleset Time Controller
		/// �A�̭��sruleset info�A�~�ઽ���qtime controller�̭���Xruleset��keybinding
		/// �{�b�i�o��A���\�b�ۤv�̭��N�n�C�H��n�ק�action�Minput��mapping���n�O�o�i�ӳo���
		/// </summary>
		virtual map<PianoAction, InputKey>* GetDefaultkeyBindings();


	protected:


		/// <summary>
		/// ��input key�M�s����J���X�@�U
		/// </summary>
		virtual int reloadMappings();

	};


}
}





#endif