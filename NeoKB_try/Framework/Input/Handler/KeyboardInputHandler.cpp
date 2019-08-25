#include "KeyboardInputHandler.h"


using namespace Framework::Input::Handler;
using namespace Framework::Host;
using namespace std;



int KeyboardInputHandler::Initialize(GameHost * host)
{
	// 當mian interface讀到輸入時，就會call handle key down來處理
	host->GetMainInterface()->GetKeyboard()->
		AddOnKeyDown<KeyboardInputHandler>(this, bind((int(KeyboardInputHandler::*)(InputState*))&KeyboardInputHandler::HandleState, this, placeholders::_1), "KeyboardInputHandler::HandleState");

	return 0;
}

int KeyboardInputHandler::HandleState(InputState * inputState)
{
	auto keyboardState = inputState->GetKeyboardState()->Clone();

	InputState* pendingState = new InputState();
	pendingState->SetKeyboardState(keyboardState);

	pendingStates.push_back(pendingState);

	return 0;
}
