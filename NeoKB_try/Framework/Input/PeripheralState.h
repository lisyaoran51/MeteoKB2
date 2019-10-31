#ifndef PERIPHERAL_STATE_H
#define PERIPHERAL_STATE_H

#include <stdexcept>

namespace Framework {
namespace Input {

	template<class T>
	class PeripheralState {

	public:

		virtual T* Clone() = 0;

		int SetIsLastState() {
			isLastState = true;
			delete lastState;
			lastState = nullptr;
			return 0;
		}

		virtual int SetLastState(T* state) {
			lastState = state;
			state->SetIsLastState();
			return 0;
		}

		virtual T* GetLastState() {
			return lastState;
		}

	protected:

		T* lastState;

		bool isLastState = false;

	};


}}





#endif