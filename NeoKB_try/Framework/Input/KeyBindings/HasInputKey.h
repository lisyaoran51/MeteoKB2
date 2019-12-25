#ifndef HAS_INPUT_KEY_H
#define HAS_INPUT_KEY_H


#include "InputKey.h"

namespace Framework {
namespace Input {
namespace KeyBindings {

	class HasInputKey {

	public:

		HasInputKey(InputKey iKey, int v);

		virtual InputKey GetInputKey();

		virtual int GetInputValue();

	protected:

		InputKey inputKey = InputKey::None;

		int inputValue = 0;

	};

}}}






#endif