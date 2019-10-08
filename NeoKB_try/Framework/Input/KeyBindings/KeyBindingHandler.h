#ifndef KEY_BINDING_HANDLER_H
#define KEY_BINDING_HANDLER_H

#include <utility>


using namespace std;

namespace Framework {
namespace Input {
namespace KeyBindings {

	template<typename T>
	class KeyBindingHandler {

	public:

		virtual OnKeyDown(pair<T, int> action) = 0;

		virtual OnKeyUp(T> action) = 0;

		virtual OnButtonDown(T action) = 0;

		virtual OnButtonUp(T action) = 0;

		virtual OnKnobTurn(pair<T, int> action) = 0;

		virtual OnSlide(pair<T, int> action) = 0;


	};

}}}


#endif