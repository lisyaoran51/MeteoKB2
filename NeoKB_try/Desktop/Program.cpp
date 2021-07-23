#include "Program.h"

#include "Hosts/MeteoGameHost.h"
#include "MeteoGameDesktop.h"
#include "../Instruments/CompositeMeteoPiano.h"
#include "../Util/Log.h"
#include "../Util/ProgramInitializer.h"



using namespace Desktop;
using namespace Desktop::Hosts;
using namespace Instruments;
using namespace Util;


Program::Program(){}


int Program::Main(vector<string>& args)
{
	LOG(LogLevel::Info) << "Program::Main() : Start program.";


	int policy = SCHED_OTHER;
	struct sched_param param;
	memset(&param, 0, sizeof(param));
	param.sched_priority = sched_get_priority_min(policy);
	pthread_setschedparam(pthread_self(), policy, &param);

	ProgramInitializer::Initialize();

	Host* host = new MeteoGameHost();
	host->Initialize("/home/pi");
	
	
	host->Run(new MeteoGameDesktop(args), new CompositeMeteoPiano(args));

	return 0;
}
