#include "../Framework/Hosts/MeteoGameHost.h"

#include "../Framework/../Desktop/Devices/MeteoBluetoothDevice.h"
#include "../Framework/../Desktop/Devices/MeteoSpidevDisplayDevice.h"
#include "../Framework/../Desktop/Devices/MeteoKeyboardDevice.h"
#include "../Framework/../Desktop/Devices/MeteoPanelDevice.h"
#include "../Framework/Input/Handler/BluetoothInputHandler.h"
#include "../Framework/Input/Handler/KeyboardInputHandler.h"
#include "../Framework/Input/Handler/PanelInputHandler.h"
#include "../Framework/IO/PlatformStorage.h"


LogLevel Util::logLevel = LogLevel::Finest;

int main() {

	Host* host = new MeteoGameHost();
	host->Initialize();

	vector<pair<uintptr_t, string>>* callbackKeys = host->GetMainInterface()->GetKeyboard()->_DebugGetActionList()->_DebugGetCallbackKeys();

	LOG(LogLevel::Fine) << "TestCaseKeyboard : callback2 keys size = " << callbackKeys->size();

	return 0;

}