#include "SheetmusicSelectPanel.h"


using namespace Games::UI;



int SheetmusicSelectPanel::load()
{
	LOG(LogLevel::Info) << "SheetmusicSelectPanel::load : start loading the Sheetmusic Panel.";

	FrameworkConfigManager * f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int SheetmusicSelectPanel::load : FrameworkConfigManager not found in cache.");

	SmManager * s = GetCache<SmManager>("SmManager");
	if (!s)
		throw runtime_error("int SheetmusicSelectPanel::load : SmManager not found in cache.");

	return load(f, s);
}

int SheetmusicSelectPanel::load(FrameworkConfigManager * f, SmManager * s)
{
	isPresent = true;

	smManager = s;

	return 0;
}

SheetmusicSelectPanel::SheetmusicSelectPanel(): RegisterType("SheetmusicSelectPanel")
{
	isInputable = true;

	registerLoad(bind(static_cast<int(SheetmusicSelectPanel::*)(void)>(&SheetmusicSelectPanel::load), this));
}

int SheetmusicSelectPanel::onCommand(InputState * inputState, InputKey command)
{

	if (command == InputKey::Bluetooth) {
		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++) {

			if (inputState->GetBluetoothState()->GetCommands()->at(i)->CommandType == "Download") {

				// ¤U¸üºq¦±...

			}

		}

		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++) {

			if (inputState->GetBluetoothState()->GetCommands()->at(i)->CommandType == "Select") {

				// ¿ï¾Üºq¦±...
				SmInfo* smInfo = nullptr;

				LOG(LogLevel::Info) << "SheetmusicSelectPanel::onCommand : song selected.";
				SelectionChanged(smInfo);

			}

		}



		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++){
			if (inputState->GetBluetoothState()->GetCommands()->at(i)->CommandType == "Start") {

				isPresent = false;

				LOG(LogLevel::Info) << "SheetmusicSelectPanel::onCommand : game start.";
				StartRequest();

			}
		}
	}
	return 0;
}
