#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H



#include <vector>
#include "Schedulable.h"
#include "../../Input/InputState.h"
#include "../../Input/KeyBindings/InputKey.h"

using namespace std;
using namespace Framework::Input;
using namespace Framework::Input::KeyBindings;


namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// every class inherit form this has a function to update. �C��update���׳Qparent�I�s�A�̫��update thread�ӥsroot
	/// </summary>
	class Triggerable : public Schedulable {

		/// <summary>
		/// �N��ثe�O�_�n���L����trigger�A�O�i�H�H�ɶ}�_������
		/// </summary>
		bool isAvailableForTrigger = true;

		bool isMaskingTrigger = false;

		bool previousIsAvailableForTrigger = isAvailableForTrigger;

		vector<ChildAddable*> cacheChilds;

	public:

		Triggerable();

		/// <summary>
		/// �мgchild addable�A�M��[�JisValidForTrigger�A�n���S�o�ӥ��n�A�R��
		/// </summary>
		//virtual int AddChild(ChildAddable* child);

		/// <summary>
		/// �мgchild addable�A�M��[�JisValidForTrigger�A�n���S�o�ӥ��n�A�R��
		/// </summary>
		//virtual int DeleteChild(ChildAddable* child);

		
		int SetIsAvailabledForTrigger(bool value);

		int MaskTrigger();

		int UnmaskTrigger();

		/// <summary>
		/// TODO: ���ӭn�令��stack�Ӧs�W�@�Ӫ��A�A���M�h��set�H��N�S��krecover
		/// </summary>
		int SetAllChildsIsMaskedForTrigger();

		int RecoverAllChildsIsMaskedForTrigger();

		bool GetIsAvailableForTrigger();

		bool GetIsInputable();

		bool GetIsPresent();

		bool GetIsInputReceivable();

		virtual int TriggerOnKeyDown(InputState* inputState, InputKey key) final;
		
		virtual int TriggerOnKeyUp(InputState* inputState, InputKey key) final;
		
		virtual int TriggerOnKnobTurn(InputState* inputState, InputKey knob) final;
		
		virtual int TriggerOnButtonDown(InputState* inputState, InputKey button) final;
		
		virtual int TriggerOnButtonUp(InputState* inputState, InputKey button) final;
		
		virtual int TriggerOnSlide(InputState* inputState, InputKey slider) final;

		virtual int TriggerOnMessage(InputState* inputState, BluetoothMessage* message) final;

	protected:

		/// <summary>
		/// �N��o�Ӫ��󥻨��i���i�H��J�A�b�ͦ����ɭԴN�����M�w
		/// </summary>
		bool isInputable = false;

		/// <summary>
		/// �N��o�Ӫ���O�_�{�b���b�ϥΡA�L�k����A���]�������ɦ۰ʧ���
		/// </summary>
		bool isPresent = false;


		virtual int onKeyDown(InputState* inputState, InputKey key);

		virtual int onKeyUp(InputState* inputState, InputKey key);

		virtual int onKnobTurn(InputState* inputState, InputKey knob);

		virtual int onButtonDown(InputState* inputState, InputKey button);

		virtual int onButtonUp(InputState* inputState, InputKey button);

		virtual int onSlide(InputState* inputState, InputKey slider);

		virtual int onMessage(InputState* inputState, BluetoothMessage* message);

	};

}}}







#endif