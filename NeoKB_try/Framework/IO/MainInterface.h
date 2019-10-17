#ifndef MAIN_INTERFACE_H
#define MAIN_INTERFACE_H

#include <vector>
#include "../Allocation/Hierachal/Container.h"


namespace Framework {
namespace Devices{
	class Device;
	class InputDevice;
	class OutputDevice;
}}


using namespace std;
using namespace Framework::Devices;
using namespace Framework::Allocation::Hierachal;



namespace Framework {
namespace IO {

	class Peripheral;
	class Display;
	class Panel;
	class BluetoothPhone;
	class Keyboard;

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class MainInterface : public Container {

		/* 這些是軟體端的device，不用考量硬體部分 */
		vector<Peripheral*> peripherals;

		Display* display;
		Keyboard* keyboard;
		Panel* panel;
		BluetoothPhone* phone;

		/* 這些是硬體端，他們在作用時會自己去取得main interface裡面的裝置 */
		vector<InputDevice*> inputDevices;
		vector<OutputDevice*> outputDevices;
		vector<Device*> devices;

	public:

		MainInterface();

		int Initialize();

		int ScanInput();

		int ProcessOutput();

		vector<Peripheral*>* GetPeripherals();

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

		ActionList<int, Peripheral*> onPeripheralRegister;
		//ActionList<int(Device*)> onDeviceRegister;

	private:




	};



}}


#endif