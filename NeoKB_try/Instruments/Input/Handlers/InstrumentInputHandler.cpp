#include "InstrumentInputHandler.h"

#include "../../../Framework/IO/Keyboard.h"
#include "../../MeteoPiano.h"
#include <algorithm>


using namespace Instruments::Input::Handlers;
using namespace Framework::IO;
using namespace Instruments;


InstrumentInputHandler::InstrumentInputHandler(Piano* p)
{
	piano = p;
}

int InstrumentInputHandler::Initialize(GameHost * host)
{
	LOG(LogLevel::Info) << "InstrumentInputHandler::Initialize() : adding handler.";

	//vector<pair<uintptr_t, string>>* callbackKeys = host->GetMainInterface()->GetKeyboard()->_DebugGetActionList()->_DebugGetCallbackKeys();
	//
	//LOG(LogLevel::Depricated) << "TestCaseKeyboard : callback keys size = " << callbackKeys->size();
	
	keyBindings = piano->GetDefaultkeyBindings();

	// 當mian interface讀到輸入時，就會call handle key down來處理
	host->GetMainInterface()->GetKeyboard()->
		AddOnKeyEvent<InstrumentInputHandler>(this, bind((int(InstrumentInputHandler::*)(InputState*))&InstrumentInputHandler::HandleState, this, placeholders::_1), "InstrumentInputHandler::HandleState");



	return 0;
}

int InstrumentInputHandler::HandleState(InputState * inputEvent)
{

	if (inputEvent->GetKeyboardState()->CheckIsEmpty())
		return 0;

	KeyboardState* keyboardState = inputEvent->GetKeyboardState();

	// 我們不考慮repeat，鎖以不用寫得很複雜
	// 暫時也不寫組合鍵，以後再寫

	for (int i = 0; i < keyboardState->GetPresses()->size(); i++) {

		pair<InputKey, int> newKey = keyboardState->GetPresses()->at(i);
		vector<KeyBinding*>::iterator it = find_if(keyBindings->begin(), keyBindings->end(), [&newKey](KeyBinding* k)->bool {
			return k->Key == newKey.first;
		});

		if (it != keyBindings->end()) {

			piano->OnDirectKeyDown(pair<PianoAction, int>((*it)->GetAction<PianoAction>(), newKey.second));
			
		}
		else {
			LOG(LogLevel::Debug) << "InstrumentInputHandler::HandleState() : key binding [" << (int)newKey.first << "] not found.";
		}
	}

	return 0;
}
