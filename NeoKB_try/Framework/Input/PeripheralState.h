#ifndef PERIPHERAL_STATE_H
#define PERIPHERAL_STATE_H

#include <stdexcept>

namespace Framework {
namespace Input {

	template<class T>
	class PeripheralState {

	public:

		virtual ~PeripheralState() = default;

		virtual T* Clone() = 0;

		int SetIsLastState() {
			isLastState = true;
			if(lastState!= nullptr)
				delete lastState;
			lastState = nullptr;
			return 0;
		}

		virtual int SetLastState(T* state) {
			lastState = state;
			if(state != nullptr)
				state->SetIsLastState();
			return 0;
		}

		virtual T* GetLastState() {
			return lastState;
		}

		virtual bool CheckIsEmpty() = 0;

	protected:

		T* lastState = nullptr;

		bool isLastState = false;

	};


}}





#endif