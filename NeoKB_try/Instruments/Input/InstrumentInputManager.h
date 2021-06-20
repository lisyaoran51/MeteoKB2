#ifndef INSTRUMENT_INPUT_MANAGER_H
#define INSTRUMENT_INPUT_MANAGER_H

#include "../../Framework/Input/Messages/MessageInputManager.h"
#include "PianoAction.h"
#include "../Instrument.h"


using namespace Framework::Input::Messages;
using namespace Instruments;


namespace Instruments {
namespace Input{

	template<typename T, typename TMessage>
	class InstrumentInputManager : public MessageInputManager<T, TMessage> {

	public:

		InstrumentInputManager(TInstrument<T>* i) : RegisterType("InstrumentInputManager") {
			instrument = i;
		}

		virtual vector<KeyBinding*>* GetDefaultkeyBindings() {
			vector<KeyBinding*>* bindings = instrument->GetDefaultkeyBindings();

			return bindings;
		}

	protected:

		TInstrument<T>* instrument;

	};


}}







#endif
