#ifndef BLUETOOTH_PHONE_H
#define BLUETOOTH_PHONE_H



#include <vector>
#include "Peripheral.h"
#include "../../Util/DataStructure/Action.h"
#include "../Input/InputState.h"

using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Input;

namespace Framework {
namespace IO {

	/// <summary>
	/// �����J���a��A�ѥX���a��
	/// </summary>
	class BluetoothPhone : public Peripheral {


	public:

		template<class _Type>
		int AddOnCommand(_Type* callableObject, function<int(InputState*)> callback, string name = "HandleCommand");

	protected:


	private:

		Action<int(InputState*)> OnCommand;


	};


}}









#endif