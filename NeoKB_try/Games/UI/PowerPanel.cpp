#include "PowerPanel.h"

using namespace Games::UI;


int PowerPanel::load()
{
	LOG(LogLevel::Info) << "PowerPanel::load() : Start loading Power panel.";

	OutputManager* o = GetCache<OutputManager>("OutputManager");

	if (!o)
		throw runtime_error("int MainMenu::load() : OutputManager not found in cache.");

	return load(o);
}

int PowerPanel::load(OutputManager * o)
{
	outputManager = o;

	isPresent = true;

	return 0;
}

PowerPanel::PowerPanel(): RegisterType("PowerPanel")
{
	// functional�̭���bind�����overloading���D
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(PowerPanel::*)())&PowerPanel::load, this));

	isInputable = true;
}

int PowerPanel::onButtonDown(InputState * inputState, InputKey button)
{
	if (button == InputKey::Power) {

		LOG(LogLevel::Debug) << "PowerPanel::onButtonDown() : power button pressed.";

		double currentTime = GetClock()->GetCurrentTime();

		if (currentTime - 4 > lastSwitchTime) {

			MeteoContextBluetoothMessage* powerMessage = new MeteoContextBluetoothMessage(MeteoCommand::KeyboardIOEvent);

			json context;
			

			if (isPowerOn) {
				PowerOffRequest();
				isPowerOn = false;

				// TODO: �R���Ҧ�ble request

				context["Event"].push_back(string("1000, 0"));	// power:1000 ����
			}
			else {
				PowerOnRequest();
				isPowerOn = true;

				context["Event"].push_back(string("1000, 1"));	// power:1000 �}��
			}

			powerMessage->SetContextInJson(context);
			powerMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

			outputManager->PushMessage(powerMessage);

			lastSwitchTime = currentTime;
		}

	}


	return 0;
}
