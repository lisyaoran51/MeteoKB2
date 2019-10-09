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

		virtual int OnKeyDown(pair<T, int> action) = 0;

		virtual int OnKeyUp(T action) = 0;

		virtual int OnButtonDown(T action) = 0;

		virtual int OnButtonUp(T action) = 0;

		virtual int OnKnobTurn(pair<T, int> action) = 0;

		virtual int OnSlide(pair<T, int> action) = 0;


	};

}}}


#endif