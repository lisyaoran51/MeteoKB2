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
	LOG(LogLevel::Info) << "Program::Main() : Start program.";

	GameHost* host = new MeteoGameHost();
	host->Initialize();

	//host->Run(new MeteoGameDesktop(args), new MeteoPiano(args));
	while (1);

	return 0;
}
