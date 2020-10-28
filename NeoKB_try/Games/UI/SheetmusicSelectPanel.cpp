#include "SheetmusicSelectPanel.h"

#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"

using namespace Games::UI;
using namespace Games::Output::Bluetooths;



int SheetmusicSelectPanel::load()
{
	LOG(LogLevel::Info) << "SheetmusicSelectPanel::load : start loading the Sheetmusic Panel.";

	FrameworkConfigManager * f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int SheetmusicSelectPanel::load : FrameworkConfigManager not found in cache.");

	SmManager * s = GetCache<SmManager>("SmManager");
	if (!s)
		throw runtime_error("int SheetmusicSelectPanel::load : SmManager not found in cache.");

	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("int SheetmusicSelectPanel::load : OutputManager not found in cache.");

	return load(f, s, o);
}

int SheetmusicSelectPanel::load(FrameworkConfigManager * f, SmManager * s, OutputManager* o)
{
	LOG(LogLevel::Info) << "int SheetmusicSelectPanel::load() : insert sheetmusic manager.";

	isPresent = true;

	smManager = s;

	frameworkConfigManager = f;

	outputManager = o;

	selectedModifiers.SetValue(new vector<Modifier*>());

	return 0;

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

BindablePointer<vector<Modifier*>*>* SheetmusicSelectPanel::GetSelectedModifiers()
{
	return &selectedModifiers;
}

int SheetmusicSelectPanel::AddModifier(Modifier * modifier)
{
	// TODO: 檢查有沒有衝突的modifier，然後把衝突的modifier刪掉
	selectedModifiers.GetValue()->push_back(modifier);
	return 0;
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

	LOG(LogLevel::Debug) << "SheetmusicSelectPanel::OnCommand() : got new bt command. ";

	if (command->GetCommand() == MeteoCommand::WriteHardwareConfiguration) {
		for (int i = 0; i < command->GetContext()["Configurations"].size(); i++) {
			if (command->GetContext()["Configurations"].at(i)["category"].get<string>() == "Framework" &&
				command->GetContext()["Configurations"].at(i)["Object"].get<string>() == "TargetHeight") {
				int height = command->GetContext()["Configurations"].at(i)["Value"].get<int>();

				frameworkConfigManager->Set(FrameworkSetting::TargetHeight, height); // 不是10就是15，對齊黑鍵或是黑白鍵不同高
			}
		}
	}

	if (command->GetCommand() == MeteoCommand::WriteGameConfiguration) {
		vector<string> modifierIds;
		string modifierName;
		int value1, value2;
		for (int i = 0; i < command->GetContext()["Modifiers"].size(); i++) {
			modifierName = command->GetContext()["Modifiers"].at(i)["Modifier Name"].get<string>() == "AutoPedalModifier";
			value1 = command->GetContext()["Modifiers"].at(i)["Value1"].get<int>();
			value2 = command->GetContext()["Modifiers"].at(i)["Value2"].get<int>();

			InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
				
			LOG(LogLevel::Fine) << "SheetmusicSelectPanel::OnCommand() : select autopedal modifier";

			Modifier* modifier = iCreator.CreateInstanceWithT<Modifier>(modifierName);
			modifier->SetValue(value1, value2);

			if (AddModifier(modifier))
				modifierIds.push_back(command->GetContext()["Modifiers"].at(i)["Modifier Id"].get<string>());

		}
		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::ReturnWriteGameConfiguration);

		meteoContextBluetoothMessage->GetContext()["Scene"] = string("SongSelect");
		for (int i = 0; i < modifierIds.size(); i++) {
			json modifierId;
			modifierId["Modifier Id"] = modifierIds[i];
			meteoContextBluetoothMessage->GetContext()["Status"].push_back(modifierId);
		}

		outputManager->PushMessage(meteoContextBluetoothMessage);
	}



	// 這一段要在開始傳檔之前送，確認琴裡面有沒有這首歌
	if (command->GetCommand() == MeteoCommand::SheetmusicData) {
		int songId = command->GetContext()["Id"].get<int>();

		vector<SmInfo*>* sInfos = smManager->GetSmInfos();
		for (int i = 0; i < sInfos->size(); i++) {
			if (sInfos->at(i)->id == songId) {
				// 回傳已有這首曲子
				MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckSheetmusicData);
				meteoContextBluetoothMessage->GetContext()["Status"] = short(-1);
				meteoContextBluetoothMessage->GetContext()["Id"] = int(songId);

				outputManager->PushMessage(meteoContextBluetoothMessage);
				return 0;
			}
		}
		// 回傳沒有這首曲子
		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckSheetmusicData);
		meteoContextBluetoothMessage->GetContext()["Status"] = short(0);
		meteoContextBluetoothMessage->GetContext()["Id"] = int(songId);

		outputManager->PushMessage(meteoContextBluetoothMessage);
		return 0;
	}

	if (command->GetCommand() == MeteoCommand::RequestLoadGame) {

		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::OnCommand() : request load game. " << command->GetContext().dump();

		string fileName = command->GetContext()["FileName"].get<string>();

		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::OnCommand() : selected file name [" << fileName << "].";

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckRequestLoadGame);
		meteoContextBluetoothMessage->GetContext()["Status"] = short(0);

		outputManager->PushMessage(meteoContextBluetoothMessage);

		vector<SmInfo*>* sInfos = smManager->GetSmInfos();
		for (int i = 0; i < sInfos->size(); i++) {
			LOG(LogLevel::Fine) << "int SheetmusicSelectPanel::OnCommand() : searching song name [" << sInfos->at(i)->fileName << "] = [" << fileName << "].";
			if (sInfos->at(i)->fileName == fileName) {

				SelectionChanged(sInfos->at(i));

				StartRequest();
				return 0;
			}
		}
		LOG(LogLevel::Error) << "int SheetmusicSelectPanel::OnCommand() : song not found.";
	}


	return -1;
}

int SheetmusicSelectPanel::onCommand(InputState * inputState, InputKey command)
{

	return 0;
}
