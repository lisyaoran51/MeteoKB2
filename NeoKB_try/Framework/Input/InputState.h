#ifndef INPUT_STATE_H
#define INPUT_STATE_H


#include <vector>
#include "BluetoothState.h"
#include "KeyboardState.h"
#include "PanelState.h"

using namespace std;


namespace Framework {
namespace Input {

	/// <summary>
	/// 當做參數送給有註冊的delegate函式
	/// </summary>
	class InputState {

	public:


		InputState* Clone();

		InputState* SetLastState(InputState* lState);

		InputState* GetLastState();

		InputState* SetKeyboardState(KeyboardState* kState);

		KeyboardState* GetKeyboardState();

		InputState* SetPanelState(PanelState* pState);

		PanelState* GetPanelState();

		InputState* SetBluetoothState(BluetoothState* bState);

		BluetoothState* GetBluetoothState();

	protected:


	private:

		InputState* lastState;

		KeyboardState* keyboardState;

		PanelState* panelState;

		BluetoothState* bluetoothState;

	};


}}

#endif