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
		songTitle = "GirlTellMe";

	vector<SmInfo*>* smInfos = smManager->GetSmInfos();
	LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : there's [" << smInfos->size() << "] songs in sm manager.";

	for (int i = 0; i < smInfos->size(); i++) {

		LOG(LogLevel::Fine) << "--------------------------------- " << smInfos->at(i)->metadata->Title;

		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : song [" << songTitle << "] is [" << smInfos->at(i)->metadata->Title << "]?";

		//if (smInfos->at(i)->metadata->Title == songTitle) {
		if (smInfos->at(i)->metadata->Title == songTitle) { // 應該要寫一個metedata，不過這邊方便，先暫時用filename就好

			LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : song [" << songTitle << "] found.";

			SelectionChanged(smInfos->at(i));
			//delete smInfos; //這個現在不能delete，因為現在改成寫死，之後改回來從檔案讀的時候才能delete


			//StartRequest();
			return 0;

			//GetScheduler()->AddDelayedTask([=]() {
			//
			//	SelectionChanged(smInfos->at(i));
			//	delete smInfos;
			//	StartRequest();
			//	return 0;
			//
			//}, 0.5);
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
