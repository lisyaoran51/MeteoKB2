#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include "Device.h"
#include <vector>
#include "../Input/InputState.h"

using namespace Framework::Input;
using namespace std;

namespace Framework {
namespace Devices {

	class InputDevice : virtual public Device {


	public:

		/// <summary>
		/// �Ainput thread�C�@���ȧΤ@���A���Pdevice�ۤv�h�w�o�@���L�n������ѤJ
		/// </summary>
		virtual int ScanInput();

	protected:

		vector<InputState*> inputStates;

		virtual int readFromDevice() = 0;

		/// <summary>
		/// �o�Ӧbinput thread
		/// PanelDevice.ScanInput => PanelDevice.PassToPeripheral => Panel.TriggerOnInput => PanelInputHandler.HandleState =>PanelInputHandler.Pushback(states)
		/// </summary>
		virtual int passToPeripheral(Peripheral* peripheral);

	};

}}




#endif