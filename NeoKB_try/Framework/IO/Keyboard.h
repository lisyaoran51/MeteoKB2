#ifndef KEYBOARD_H
#define KEYBOARD_H



#include <vector>
#include "Peripheral.h"
#include "../../Util/DataStructure/ActionList.h"
#include "../Input/InputState.h"

namespace Framework {
namespace Devices{
	class Device;
	class KeyboardDevice;
}}


using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Input;
using namespace Framework::Devices;

namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class Keyboard : public Peripheral {


	public:

		virtual int SetDevice(Device* device);

		virtual int TriggerOnInput();

		template<class _Type>
		int AddOnKeyEvent(_Type* callableObject, function<int(InputState*)> callback, string name = "HandleKeyEvent");

	protected:

		KeyboardDevice* matchedKeyboardDevice;

	private:

		ActionList<int, InputState*> OnKeyEvent;

	};


}}









#endif