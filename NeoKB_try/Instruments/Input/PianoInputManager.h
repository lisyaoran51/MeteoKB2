#ifndef PIANO_INPUT_MANAGER_H
#define PIANO_INPUT_MANAGER_H


#include "InstrumentInputManager.h"
#include "PianoAction.h"
#include "../Piano.h"


using namespace Instruments;


namespace Instruments {
namespace Input{

	class PianoInputManager : public InstrumentInputManager<PianoAction> {

	public:

		PianoInputManager(Piano* p);

	};


}}





#endif