#ifndef KEY_BINDING_H
#define KEY_BINDING_H

#include <string>
#include "InputKey.h"


using namespace std;


namespace Framework {
namespace Input {
namespace KeyBindings {

	struct KeyBinding {

		InputKey Key;
		int action;

		KeyBinding(InputKey k, int a) {
			Key = k;
			action = a;
		}

		template<typename T>
		T GetAction() {
			return (T)action;// dynamic_cast<T>(action);
		}

		string ToString() {
			return to_string((int)Key);
		}


	};



}}}



#endif