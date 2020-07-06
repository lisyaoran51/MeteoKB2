#ifndef METEOR_TIME_CONTROLLER_H
#define METEOR_TIME_CONTROLLER_H

#include "../../Framework/Timing/TimeController.h"
#include "../Input/MeteorInputManager.h"


using namespace Meteor::Input;
using namespace Framework::Timing;


namespace Meteor {
namespace Timing {

	/// <summary>
	/// �C�����O���ӭn�\�b�O���a��A���ȮɷQ�B�D�n�\���A���\�o��@�U
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
		/// �n����C�Ӥp�`����ơA����A��p�`�۶s�ɤ~�����諸��m�C�p�G�S�����p�`��ƪ��ܡA�p�`�۶s�N�Τ@��3��
		/// �|�Ameteor ruleset executor�̭��]�w
		/// </summary>
		int SetSectionTime(vector<float>* sTime);

		int SetTimeControllerMode(MeteorTimeControllerMode tControllerMode);

		int SetRepeatSections(int rSections);

		int SetHasSection(bool hSection);

		/// <summary>
		/// TODO:
		/// ����action�Minput��map�A�o�����ӭn�\�bMeteor Ruleset�~��A����n���L�h�A�M�ᤤ���a�@�hRuleset Time Controller
		/// �A�̭��sruleset info�A�~�ઽ���qtime controller�̭���Xruleset��keybinding
		/// �{�b�i�o��A���\�b�ۤv�̭��N�n�C�H��n�ק�action�Minput��mapping���n�O�o�i�ӳo���
		/// </summary>
		//virtual map<MeteorAction, InputKey>* GetDefaultkeyBindings(); // �令���~��meteo action


	protected:

		MeteorTimeControllerMode timeControllerMode = MeteorTimeControllerMode::MusicGame;

		bool hasSection = false;

		int repeatSections = -1;

		vector<float> sectionTime;

		EventProcessorFilter* eventProcessorFilter = nullptr;

		/// <summary>
		/// ��input key�M�s����J���X�@�U
		/// </summary>
		//virtual int reloadMappings(); // �令���~��meteo action

		virtual int onButtonDown(InputState* inputState, InputKey button);

		virtual int onKnobTurn(InputState* inputState, InputKey knob);

	};


}
}





#endif