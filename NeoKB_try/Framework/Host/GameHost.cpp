#include "GameHost.h"

using namespace Framework::Host;

int GameHost::ResetInputHandler()
{

	availableInputHandler.push_back(pianoKeyInputHandler);

	// �binitialize�̭��A�|��ۤv��on input���U��host.OnInput��
	pianoKeyInputHandler->Intialize();

	return 0;
}
