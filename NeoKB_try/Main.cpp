#include "Desktop/Program.h"

#include <vector>
#include <string>



using namespace std;
using namespace Desktop;



int main(int argc, char *argv[]) {

	vector<string> args;

	for (int i = 0; i < argc; i++) {
		args.push_back(argv[i]);
	}

	Program::Main(args);

	return 0;

}