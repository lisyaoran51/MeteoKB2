#include "BackgroundBleAccess.h"


using namespace Games::IO::Communications;
using namespace Framework::Threading;


BackgroundBleAccess::BackgroundBleAccess(Host * gHost): TBleAccess<BackgroundGetBinaryBleRequest>(gHost), RegisterType("BackgroundBleAccess")
{
	
}
