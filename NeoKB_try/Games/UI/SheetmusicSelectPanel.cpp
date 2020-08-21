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

	frameworkConfigManager = f;

	// 暫時先這樣 之後再改
	string songTitle;
	if (!f->Get(FrameworkSetting::SongTitle, &songTitle))
		songTitle = "GirlTellMe";

	vector<SmInfo*>* sInfos = smManager->GetSmInfos();
	LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : there's [" << sInfos->size() << "] songs in sm manager.";

	for (int i = 0; i < sInfos->size(); i++) {

		LOG(LogLevel::Fine) << "--------------------------------- " << sInfos->at(i)->metadata->Title;

		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : song [" << songTitle << "] is [" << sInfos->at(i)->metadata->Title << "]?";

		//if (smInfos->at(i)->metadata->Title == songTitle) {
		if (sInfos->at(i)->metadata->Title == songTitle) { // 應該要寫一個metedata，不過這邊方便，先暫時用filename就好

			LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : song [" << songTitle << "] found.";

			SelectionChanged(sInfos->at(i));
			//delete smInfos; //這個現在不能delete，因為現在改成寫死，之後改回來從檔案讀的時候才能delete


			StartRequest();
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

int SheetmusicSelectPanel::SetSms(vector<SmInfo*>* sInfos)
{
	if (smInfos != nullptr)
		delete smInfos;
	smInfos = sInfos;

	return 0;
}

int SheetmusicSelectPanel::OnCommand(MeteoBluetoothCommand * command)
{
	if (command->GetCommand() == MeteoCommand::WriteHardwareConfiguration) {
		for (int i = 0; i < command->GetContext()["Configurations"].size(); i++) {
			if (command->GetContext()["Configurations"].at(i)["category"].get<string>() == "Framework" &&
				command->GetContext()["Configurations"].at(i)["Object"].get<string>() == "TargetHeight") {
				int height = command->GetContext()["Configurations"].at(i)["Value"].get<int>();

				frameworkConfigManager->Set(FrameworkSetting::TargetHeight, height); // 不是10就是15，對齊黑鍵或是黑白鍵不同高
			}
		}

		

	}


	// 這一段要在開始傳檔之前送，確認琴裡面有沒有這首歌
	if (command->GetCommand() == MeteoCommand::SheetmusicData) {
		int songId = command->GetContext()["Id"].get<int>();

		vector<SmInfo*>* sInfos = smManager->GetSmInfos();
		for (int i = 0; i < sInfos->size(); i++) {
			if (sInfos->at(i)->id == songId) {
				// 回傳已有這首曲子


				return 0;
			}
			else {
				// 回傳沒有這首曲子
				return 0;
			}
		}
	}

	if (command->GetCommand() == MeteoCommand::RequestLoadGame) {
		string fileName = command->GetContext()["FileName"].get<string>();

		vector<SmInfo*>* sInfos = smManager->GetSmInfos();
		for (int i = 0; i < sInfos->size(); i++) {
			if (sInfos->at(i)->fileName == fileName) {

				SelectionChanged(sInfos->at(i));

				StartRequest();
				return 0;
			}
		}

	}


	return -1;
}

int SheetmusicSelectPanel::onCommand(InputState * inputState, InputKey command)
{

	if (command == InputKey::Bluetooth) {
		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++) {

			//if (inputState->GetBluetoothState()->GetCommands()->at(i)->CommandType == "Download") {
			//
			//	// 下載歌曲...
			//
			//}

		}

		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++) {

			//if (inputState->GetBluetoothState()->GetCommands()->at(i)->CommandType == "Select") {
			//
			//	// 選擇歌曲...
			//	SmInfo* smInfo = nullptr;
			//
			//	LOG(LogLevel::Info) << "SheetmusicSelectPanel::onCommand : song selected.";
			//	SelectionChanged(smInfo);
			//
			//}

		}



		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++){
			//if (inputState->GetBluetoothState()->GetCommands()->at(i)->CommandType == "Start") {
			//
			//	isPresent = false;
			//
			//	LOG(LogLevel::Info) << "SheetmusicSelectPanel::onCommand : game start.";
			//	StartRequest();
			//
			//}
		}
	}
	return 0;
}
