#include "ApiAccess.h"


using namespace Framework::IO::Api;


ApiAccess::ApiAccess(GameHost * gHost): RegisterType("ApiAccess")
{

	gameHost = gHost;

	gameHost->GetMainInterface()->GetBluetoothPhone()->Add

}
