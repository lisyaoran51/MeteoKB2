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

		virtual int readFromDevice() = 0;

		virtual int passToPeripheral(Peripheral* peripheral) = 0;

	};

}}




#endif