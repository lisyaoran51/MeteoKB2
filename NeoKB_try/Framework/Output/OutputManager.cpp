#include "OutputManager.h"

#include "Bluetooths/BluetoothOutputer.h"
#include "Panels/PanelOutputer.h"


using namespace Framework::Output;
using namespace Framework::Output::Bluetooths;
using namespace Framework::Output::Panels;




OutputManager::OutputManager(MainInterface * mInterface)
{
	mainInterface = mInterface;
	bluetoothManager = new BluetoothManager();
	panelManager = new PanelManager();
	AddItemToList(bluetoothManager);
	AddItemToList(panelManager);


}

int OutputManager::PushMessage(OutputMessage * outputMessage)
{
	for (int i = 0; i < items.size(); i++) {
		items[i]->PushMessage(outputMessage);
	}
	return 0;
}

int OutputManager::AddItem(OutputComponent * item)
{
	item->SetupPeripheral(mainInterface);

	if (dynamic_cast<BluetoothOutputer*>(item))
		bluetoothManager->AddItem(item);

	if (dynamic_cast<PanelOutputer*>(item))
		panelManager->AddItem(item);

	return 0;
}
