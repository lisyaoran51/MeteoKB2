#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H



#include <vector>
#include "Schedulable.h"
#include "../../Input/InputState.h"

using namespace std;
using namespace Framework::Input;

namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// every class inherit form this has a function to update. �C��update���׳Qparent�I�s�A�̫��update thread�ӥsroot
	/// </summary>
	class Triggerable : public Schedulable {

		bool isValidForTrigger = false;


	public:

		Triggerable();

		/// <summary>
		/// �мgchild addable�A�M��[�JisValidForTrigger
		/// </summary>
		virtual int AddChild(ChildAddable* child);

		/// <summary>
		/// �мgchild addable�A�M��[�JisValidForTrigger
		/// </summary>
		virtual int DeleteChild(ChildAddable* child);

		int SetIsValidForTrigger(bool value);

		bool GetIsValidForTrigger();

		bool GetIsInputable();

		int TriggerOnKeyDown(InputState* inputState, Key key);

		int TriggerOnKeyUp(InputState* inputState, Key key);

		int TriggerOnKnobTurn(InputState* inputState, PanelKnob knob);

		int TriggerOnButtonDown(InputState* inputState, PanelButton button);

		int TriggerOnButtonUp(InputState* inputState, PanelButton button);

		int TriggerOnSlide(InputState* inputState, PanelSlider slider);

	protected:

		/// <summary>
		/// �N��o�Ӫ��󥻨��i���i�H��J�A�b�ͦ����ɭԴN�����M�w
		/// </summary>
		bool isInputable = false;

		virtual int onKeyDown(InputState* inputState, Key key);

		virtual int onKeyUp(InputState* inputState, Key key);

		virtual int onKnobTurn(InputState* inputState, PanelKnob knob);

		virtual int onButtonDown(InputState* inputState, PanelButton button);

		virtual int onButtonUp(InputState* inputState, PanelButton button);

		virtual int onSlide(InputState* inputState, PanelSlider slider);

	};

}}}







#endif