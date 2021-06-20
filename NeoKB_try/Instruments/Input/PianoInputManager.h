#ifndef PIANO_INPUT_MANAGER_H
#define PIANO_INPUT_MANAGER_H


#include "InstrumentInputManager.h"
#include "PianoAction.h"
#include "../Piano.h"
#include "../../Games/Output/Bluetooths/MeteoBluetoothMessage.h"


using namespace Instruments;
using namespace Games::Output::Bluetooths;


namespace Instruments {
namespace Input{

	class PianoInputManager : public InstrumentInputManager<PianoAction, MeteoBluetoothMessage> {

	public:

		PianoInputManager(Piano* p);

	};


}}





#endif