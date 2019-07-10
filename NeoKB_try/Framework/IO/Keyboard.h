#ifndef KEYBOARD_H
#define KEYBOARD_H



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
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class Keyboard : public Peripheral {


	public:

		template<class _Type>
		int AddOnKeyDown(_Type* callableObject, function<int(InputState*)> callback, string name = "HandleKeyDown");

	protected:


	private:

		Action<int(InputState*)> OnKeyDown;


	};


}}









#endif