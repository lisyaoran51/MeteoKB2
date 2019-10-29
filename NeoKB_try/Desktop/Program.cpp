#include "Program.h"

#include "../Framework/Host/MeteoGameHost.h"
#include "MeteoGameDesktop.h"
#include "../Instruments/MeteoPiano.h"



using namespace Desktop;
using namespace Framework::Host;
using namespace Instruments;


Program::Program(){}


int Program::Main(vector<string>& args)
{
	GameHost* host = new MeteoGameHost();

	host->Run(new MeteoGameDesktop(args), new MeteoPiano(args));


	return 0;
}
