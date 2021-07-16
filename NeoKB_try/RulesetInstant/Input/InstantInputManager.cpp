#include "InstantInputManager.h"


using namespace Instant::Input;


InstantInputManager::InstantInputManager(RulesetInfo * rInfo): RulesetInputManager(rInfo), RegisterType("InstantInputManager")
{


}

int InstantInputManager::propagateBluetoothMessage(vector<Triggerable*>* queue, InputState * state, BluetoothMessage * message)
{
	return 0;
}
