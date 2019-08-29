#ifndef KEYBOARD_H
#define KEYBOARD_H



#include <vector>
#include "Peripheral.h"
#include "../../Util/DataStructure/ActionList.h"
#include "../Input/InputState.h"

using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Input;

namespace Framework {
namespace IO {

	/// <summary>
	/// �����J���a��A�ѥX���a��
	/// </summary>
	class Keyboard : public Peripheral {


	public:

		template<class _Type>
		int AddOnKeyDown(_Type* callableObject, function<int(InputState*)> callback, string name);

	protected:


	private:

		ActionList<int(InputState*)> OnKeyDown;


	};


}}









#endif