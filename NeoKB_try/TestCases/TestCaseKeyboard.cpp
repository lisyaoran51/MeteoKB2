#include "../Framework/IO/Keyboard.h"
#include "../Framework/IO/MainInterface.h"



using namespace Framework::IO;



class handler {

public:

	int handle(InputState* is) {
		return 0;
	}

};

LogLevel Util::logLevel = LogLevel::Finest;

int main() {

	Keyboard k;

	vector<pair<uintptr_t, string>>* callbackKeys = k._DebugGetActionList()->_DebugGetCallbackKeys();

	LOG(LogLevel::Fine) << "TestCaseKeyboard : callback keys size = " << callbackKeys->size();

	handler h;

	k.AddOnKeyEvent(&h, bind((int(handler::*)(InputState*))&handler::handle, &h, placeholders::_1), "handler::handle");

	MainInterface m;

	m.Initialize();

	vector<pair<uintptr_t, string>>* callbackKeys2 = m.GetKeyboard()->_DebugGetActionList()->_DebugGetCallbackKeys();

	LOG(LogLevel::Fine) << "TestCaseKeyboard : callback2 keys size = " << callbackKeys2->size();

	return 0;

}