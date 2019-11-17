#include "../Framework/IO/Keyboard.h"


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
	handler h;

	k.AddOnKeyEvent(&h, bind((int(handler::*)(InputState*))&handler::handle, &h, placeholders::_1), "handler::handle");

	return 0;

}