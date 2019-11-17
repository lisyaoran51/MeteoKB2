
#include "ActionList.h"


using namespace Util::DataStructure;


LogLevel Util::logLevel = LogLevel::Finest;

class base {

public:

	int func(int a) {
		return 0;
	}

};


int main() {

	base b;

	ActionList<int, int> list;

	list.Add(&b,
		bind((int(base::*)(int))&base::func, &b, placeholders::_1),
		"base::func");

	return 0;

}
