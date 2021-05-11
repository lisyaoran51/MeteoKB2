#include "Desktop/Program.h"

#include "Util/Log.h"
#include <vector>
#include <string>
#include <fstream>



using namespace std;
using namespace Desktop;
using namespace Util;

LogLevel Util::logLevel = LogLevel::Info;

int main(int argc, char *argv[]) {

	LogIt::Initialize();
	Util::logLevel = LogLevel::Info;

	if(argc > 1)
		Util::logLevel = (LogLevel)atoi(argv[1]);

	// ÅªÀÉ§ìloglevel
	fstream stream;
	stream.open("./LogLevel");
	if (stream.is_open()) {
		char level;
		stream.read(&level, 1);

		if (level - '0' >= 0 && level - '0' <= 9) {
			Util::logLevel = (LogLevel)(level - '0');
		}

		stream.close();
	}

	vector<string> args;

	for (int i = 0; i < argc; i++) {
		args.push_back(argv[i]);
	}

	Program::Main(args);

	return 0;

}