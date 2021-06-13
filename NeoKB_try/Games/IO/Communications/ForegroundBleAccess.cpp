#include "ForegroundBleAccess.h"


using namespace Games::IO::Communications;


ForegroundBleAccess::ForegroundBleAccess(Host * gHost) : TBleAccess<BleRequest>(gHost), RegisterType("ForegroundBleAccess")
{
}
