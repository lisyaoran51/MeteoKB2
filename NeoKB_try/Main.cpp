#include "Desktop/Program.h"
#include "Util/Log.h"

#include <vector>
#include <string>



using namespace std;
using namespace Desktop;
using namespace Util;

LogLevel Util::logLevel = LogLevel::Info;

int main(int argc, char *argv[]) {

	Util::logLevel = LogLevel::Info;

	if(argc > 0)
		Util::logLevel = (LogLevel)atoi(argv[1]);


	vector<string> args;

	for (int i = 0; i < argc; i++) {
		args.push_back(argv[i]);
	}

	Program::Main(args);

	return 0;

}