#include "Program.h"

#include "../Framework/Host/MeteoGameHost.h"
#include "MeteoGameDesktop.h"
#include "../Instruments/MeteoPiano.h"
#include "../Util/Log.h"



using namespace Desktop;
using namespace Framework::Host;
using namespace Instruments;
using namespace Util;


Program::Program(){}


int Program::Main(vector<string>& args)
{
	Util::logLevel = LogLevel::Info;

	GameHost* host = new MeteoGameHost();

	host->Run(new MeteoGameDesktop(args), new MeteoPiano(args));


	return 0;
}
