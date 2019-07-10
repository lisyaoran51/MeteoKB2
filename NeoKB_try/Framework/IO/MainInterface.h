#ifndef MAIN_INTERFACE_H
#define MAIN_INTERFACE_H

#include <vector>
#include "Display.h"
#include "Keyboard.h"
#include "Panel.h"
#include "BluetoothPhone.h"

#include "../Devices/InputDevice.h"
#include "../Devices/OutputDevice.h"

using namespace std;
using namespace Framework::Devices;

namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class MainInterface {

		Display* display;
		Keyboard* keyboard;
		Panel* panel;
		BluetoothPhone* phone;

		vector<InputDevice*> inputDevices;

		vector<OutputDevice*> outputDevices;

	public:

		MainInterface();

		int ScanInput();

		int ProcessOutput();

		int RegisterInputDevice(InputDevice* inputDevice);
		int RegisterOutputDevice(OutputDevice* outputDevice);

		
		int RegisterDisplay(Display* d);
		Display* GetDisplay();

		int RegisterKeyboard(Keyboard* kb);
		Keyboard* GetKeyboard();

		int RegisterPanel(Panel* p);
		Panel* GetPanel();

		int RegisterBluetoothPhone(BluetoothPhone* btPhone);
		BluetoothPhone* GetBluetoothPhone();

		template<class _Type>
		int AddOnPeripheralRegister(_Type* callableObject, function<int(InputState*)> callback, string name = "HandlePeripheralRegister") {
			onPeripheralRegister.Add(callableObject, callback, name);
			return 0;
		}

	protected:

		Action<int, Peripheral*> onPeripheralRegister;
		//Action<int(Device*)> onDeviceRegister;

	private:




	};



}}


#endif