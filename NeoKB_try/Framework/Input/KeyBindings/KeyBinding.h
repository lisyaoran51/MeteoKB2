#ifndef KEY_BINDING_H
#define KEY_BINDING_H

#include <string>


using namespace std;


namespace Framework {
namespace Input {
namespace KeyBindings {

	struct KeyBinding {

		int Key;

		KeyBinding(int k) {
			Key = k;
		}

		template<typename T>
		T GetKey() {
			return dynamic_cast<T>(Key);
		}

		string ToString() {
			return to_string(Key);
		}


	};



}}}



#endif