#include "GameHost.h"

using namespace Framework::Host;

int GameHost::ResetInputHandler()
{

	availableInputHandler.push_back(pianoKeyInputHandler);

	// 在initialize裡面，會把自己的on input註冊到host.OnInput裡
	pianoKeyInputHandler->Intialize();

	return 0;
}
