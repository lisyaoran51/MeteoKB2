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
	LOG(LogLevel::Info) << "int SheetmusicSelectPanel::load() : insert sheetmusic manager.";

	isPresent = true;

	smManager = s;



	// 暫時先這樣 之後再改
	string songTitle;
	if (!f->Get(FrameworkSetting::SongTitle, &songTitle))
		songTitle = "casual";

	vector<SmInfo*>* smInfos = smManager->GetSmInfos();
	for (int i = 0; i < smInfos->size(); i++) {



		if (smInfos->at(i)->metadata->Title == songTitle) {

			LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : song [" << songTitle << "] found.";
			GetScheduler()->AddDelayedTask([=]() {

				SelectionChanged(smInfos->at(i));
				delete smInfos;
				StartRequest();
				return 0;

			}, 0.5);
		}
	}
	

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

				// 下載歌曲...

			}

		}

		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++) {

			if (inputState->GetBluetoothState()->GetCommands()->at(i)->CommandType == "Select") {

				// 選擇歌曲...
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
