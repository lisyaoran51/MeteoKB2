#ifndef INSTRUMENT_INPUT_MANAGER_H
#define INSTRUMENT_INPUT_MANAGER_H

#include "../../Framework/Input/KeyBindings/KeyBindingInputManager.h"
#include "PianoAction.h"
#include "../Instrument.h"


using namespace Framework::Input::KeyBindings;
using namespace Instruments;


namespace Instruments {
namespace Input{

	template<typename T>
	class InstrumentInputManager : public KeyBindingInputManager<T> {

	public:

		InstrumentInputManager(Instrument<T>* i) : RegisterType("InstrumentInputManager") {
			instrument = i;
		}

		virtual vector<KeyBinding*>* GetDefaultkeyBindings() {
			vector<KeyBinding*>* bindings = instrument->GetDefaultkeyBindings();

			return bindings;
		}

	protected:

		Instrument<T>* instrument;

	};


}
}







#endif
