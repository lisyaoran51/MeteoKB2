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
		/// 再input thread每一輪值形一次，不同device自己去定這一輪他要做什麼書入
		/// </summary>
		virtual int ScanInput();

	protected:

		virtual int readFromDevice() = 0;

		virtual int passToPeripheral(Peripheral* peripheral) = 0;

	};

}}




#endif