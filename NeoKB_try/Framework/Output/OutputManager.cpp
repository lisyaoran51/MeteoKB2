#include "OutputManager.h"

#include "Bluetooths/BluetoothOutputer.h"
#include "Panels/PanelOutputer.h"


using namespace Framework::Output;
using namespace Framework::Output::Bluetooths;
using namespace Framework::Output::Panels;




OutputManager::OutputManager(MainInterface * mInterface): RegisterType("OutputManager")
{
	mainInterface = mInterface;
	bluetoothManager = new BluetoothManager();
	panelManager = new PanelManager();
	AddItemToList(bluetoothManager);
	AddItemToList(panelManager);

}

int OutputManager::SetupPeripheral(MainInterface * mainInterface)
{
	return 0;
}

int OutputManager::PushMessage(OutputMessage * outputMessage)
{
	unique_lock<mutex> uLock(itemMutex);
	for (int i = 0; i < items.size(); i++) {

		LOG(LogLevel::Depricated) << "OutputManager::PushMessage : push to outputer." << i;
		items[i]->PushMessage(outputMessage);
	}
	delete outputMessage;
	outputMessage = nullptr;
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
