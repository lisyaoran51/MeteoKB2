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
	/// ���Ѽưe�������U��delegate�禡
	/// </summary>
	class InputState {

	public:


		virtual InputState* Clone();

		int SetLastState(InputState* lState);

		InputState* GetLastState();

		int SetKeyboardState(KeyboardState* kState);

		KeyboardState* GetKeyboardState();

		int SetPanelState(PanelState* pState);

		PanelState* GetPanelState();

		int SetBluetoothState(BluetoothState* bState);

		BluetoothState* GetBluetoothState();

		int SetIsLastState();

	protected:


	private:

		InputState* lastState;

		KeyboardState* keyboardState;

		PanelState* panelState;

		BluetoothState* bluetoothState;

		bool isLastState = false;

	};


}}

#endif