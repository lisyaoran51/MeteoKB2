#include "PianoInputManager.h"


using namespace Instruments::Input;

PianoInputManager::PianoInputManager(Piano* p) : RegisterType("PianoInputManager"), InstrumentInputManager<PianoAction, MeteoBluetoothMessage>(p){
}