#include "BackgroundBleAccess.h"


using namespace Games::IO::Communications;
using namespace Framework::Threading;


BackgroundBleAccess::BackgroundBleAccess(Host * gHost): TBleAccess(gHost), RegisterType("BackgroundBleAccess")
{
	
}
