#include "BluetoothPanel.h"

using namespace Games::UI;

int BluetoothPanel::load()
{

	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("int SheetmusicSelectPanel::load : OutputManager not found in cache.");
	return 0;
}

int BluetoothPanel::load(OutputManager * o)
{
	return 0;
}

BluetoothPanel::BluetoothPanel() : RegisterType("BluetoothPanel")
{
	registerLoad(bind(static_cast<int(BluetoothPanel::*)(void)>(&BluetoothPanel::load), this));
}

int BluetoothPanel::onMessage(MeteoBluetoothMessage * message)
{
	MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);

	if (contextMessage == nullptr) {
		return -1;
	}
	json context = contextMessage->GetContextInJson();

	LOG(LogLevel::Debug) << "BluetoothPanel::onMessage() : got new bt message. ";

	if (contextMessage->GetCommand() == MeteoCommand::Ping) {

		LOG(LogLevel::Fine) << "BluetoothPanel::onMessage() : ping. ";

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::Pong);

		json returnContext;

		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);

		if (!isBluetoothOn) {

			BluetoothOnRequest();
			isBluetoothOn = true;
		}

	}

	return 0;
}
