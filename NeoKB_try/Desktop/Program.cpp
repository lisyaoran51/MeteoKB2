#include "Program.h"

#include "../Framework/Host/MeteoGameHost.h"
#include "MeteoGameDesktop.h"
#include "../Instruments/CompositeMeteoPiano.h"
#include "../Util/Log.h"
#include "../Util/ProgramInitializer.h"



using namespace Desktop;
using namespace Framework::Host;
using namespace Instruments;
using namespace Util;


Program::Program(){}


int Program::Main(vector<string>& args)
{
	LOG(LogLevel::Info) << "Program::Main() : Start program.";

	ProgramInitializer::Initialize();

	GameHost* host = new MeteoGameHost();
	host->Initialize();

	
	
	host->Run(new MeteoGameDesktop(args), new CompositeMeteoPiano(args));

	return 0;
}
