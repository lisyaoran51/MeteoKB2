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
	/// �����J���a��A�ѥX���a��
	/// </summary>
	class MainInterface : public Container {

		/* �o�ǬO�n��ݪ�device�A���ΦҶq�w�鳡�� */
		vector<Peripheral*> peripherals;

		Display* display;
		Keyboard* keyboard;
		Panel* panel;
		BluetoothPhone* phone;

		/* �o�ǬO�w��ݡA�L�̦b�@�ήɷ|�ۤv�h���omain interface�̭����˸m */
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