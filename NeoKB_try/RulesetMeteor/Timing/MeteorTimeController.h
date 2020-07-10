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

	enum class RepeatPracticeMode {
		None,
		Demonstrate,
		Practice
	};

	class MeteorTimeController : public TimeController {

		int load();

		int loadOnComplete(EventProcessorFilter* e);

		/// <summary>
		/// �A���нm�u�Ҧ����A�קK�u��O���q�������A�ҨϥΪ�filter
		/// </summary>
		int filterEventBySection(vector<EventProcessor<Event>*>* eventProcessors);

		/// <summary>
		/// ���нm�u�ҵ����A�Ψӥܽd��
		/// </summary>
		int filterEruptEffect(vector<EventProcessor<Event>*>* eventProcessors);

		/// <summary>
		/// ���нm�u�ҵ����A�Ψӽm�ߪ�
		/// </summary>
		int filterFallEffect(vector<EventProcessor<Event>*>* eventProcessors);

	public:

		MeteorTimeController();

		/*---------���нm�u�M��---------*/
		/// <summary>
		/// �n����C�Ӥp�`����ơA����A��p�`�۶s�ɤ~�����諸��m�C�p�G�S�����p�`��ƪ��ܡA�p�`�۶s�N�Τ@��3��
		/// �|�Ameteor ruleset executor�̭��]�w
		/// </summary>
		int SetSectionTime(vector<float>* sTime);

		int SetTimeControllerMode(MeteorTimeControllerMode tControllerMode);

		int SetRepeatSections(int rSections);

		int SetRepeatTimes(int rTimes);

		int SetHasSection(bool hSection);

		/// <summary>
		/// ���нm�u�����Х\��
		/// </summary>
		int RepeatSection(int section);
		/*---------���нm�u�M��---------*/


	protected:

		virtual int LoadOnComplete();

		MeteorTimeControllerMode timeControllerMode = MeteorTimeControllerMode::MusicGame;

		/*---------���нm�u�M��---------*/
		bool hasSection = false;

		/// <summary>
		/// ���нm�u���}�l�p�`
		/// </summary>
		int tempStartSection = 0;

		/// <summary>
		/// ���нm�u�̦��X�p�`
		/// </summary>
		int repeatSections = 1;

		/// <summary>
		/// �ثe�u��X�p�`
		/// </summary>
		int tempSection = 0;

		/// <summary>
		/// ���нm�u�X��
		/// </summary>
		int repeatTimes = 4;

		/// <summary>
		/// �ثe���нͤF�X��
		/// </summary>
		int tempRepeatTimes = 0;

		/// <summary>
		/// �ثe�O�b�@�������ܽd�٬O�m��
		/// </summary>
		RepeatPracticeMode repeatPracticeMode = RepeatPracticeMode::None;

		/// <summary>
		/// �Ҧ��p�`�}�l���
		/// </summary>
		vector<float> sectionTime;

		float repeatBufferTime = 1;
		/*---------���нm�u�M��---------*/

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