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
	/// every class inherit form this has a function to update. 每個update都匯被parent呼叫，最後由update thread來叫root
	/// </summary>
	class Triggerable : public Schedulable {

		/// <summary>
		/// 代表目前是否要讓他接受trigger，是可以隨時開起關閉的
		/// </summary>
		bool isAvailableForTrigger = true;

		bool isMaskingTrigger = false;

		bool previousIsAvailableForTrigger = isAvailableForTrigger;

		vector<ChildAddable*> cacheChilds;

	public:

		Triggerable();

		/// <summary>
		/// 覆寫child addable，然後加入isValidForTrigger，好像沒這個必要，刪掉
		/// </summary>
		//virtual int AddChild(ChildAddable* child);

		/// <summary>
		/// 覆寫child addable，然後加入isValidForTrigger，好像沒這個必要，刪掉
		/// </summary>
		//virtual int DeleteChild(ChildAddable* child);

		
		int SetIsAvailabledForTrigger(bool value);

		int MaskTrigger();

		int UnmaskTrigger();

		/// <summary>
		/// TODO: 應該要改成用stack來存上一個狀態，不然多次set以後就沒辦法recover
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
		/// 代表這個物件本身可不可以輸入，在生成的時候就必須決定
		/// </summary>
		bool isInputable = false;

		/// <summary>
		/// 代表這個物件是否現在仍在使用，無法控制，跳夜跳場景時自動改變
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