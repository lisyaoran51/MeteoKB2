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

	class MeteorTimeController : public MeteoTimeController<MeteorAction> {

		int load();

		int loadOnComplete(EventProcessorFilter* e);

		/// <summary>
		/// �A���нm�u�Ҧ����A�קK�u��O���q�������A�ҨϥΪ�filter
		/// </summary>
		bool filterEventBySection(EventProcessor<Event>* eventProcessor);

		/// <summary>
		/// ���нm�u�ҵ����A�Ψӥܽd��
		/// </summary>
		bool filterEruptEffect(EventProcessor<Event>* eventProcessor);

		/// <summary>
		/// ���нm�u�ҵ����A�Ψӽm�ߪ�
		/// </summary>
		bool filterFallEffect(EventProcessor<Event>* eventProcessor);

	public:

		MeteorTimeController();

		virtual int SetRate(double r);

		/* ------------------------ TimeController.h ------------------------ */

		int SetLastEventOverTime(double lEventOverTime);

		virtual int OnButtonDown(MeteorAction action);

		virtual int OnKnobTurn(pair<MeteorAction, int> action);


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




		double lastEventOverTime = -1;

		MeteorTimeControllerMode timeControllerMode = MeteorTimeControllerMode::MusicGame;

		/// <summary>
		/// �bupdate���ˬd���S���Q�Ȱ��A�����ܴN��ۤv��������
		/// </summary>
		virtual int update();

		virtual int LoadOnComplete();

		/// <summary>
		/// �ˬd�C���O�_�����A�i�H�ھڹC��event�O�_�����]���ӧP�_�A�]�i�H�ھڤ��P�W�h�P�_
		/// </summary>
		virtual bool checkIsGameOver();

		/*---------���нm�u�M��---------*/ // �o�Ӥ��A�����\�Arepeat practice event processor�̭���?
		bool hasSection = false;

		/// <summary>
		/// ���нm�u���}�l�p�`
		/// </summary>
		int tempRepeatStartSection = 0;

		/// <summary>
		/// �C�@�����нm�u���t�X�p�`
		/// </summary>
		int maxSectionAmountForOneRepeat = 1;

		/// <summary>
		/// �ثe�u��X�p�`
		/// </summary>
		//int s = 0;

		/// <summary>
		/// �C�Ӵ`���n���нm�u�X��
		/// </summary>
		int maxRepeatCounts = 4;

		/// <summary>
		/// �ثe�o�Ӵ`�������нͤF�X��
		/// </summary>
		int tempRepeatCounts = 0;

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

		virtual int onMessage(MeteoBluetoothMessage* message);

	};


}
}





#endif