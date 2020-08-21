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

	// �Ȯɥ��o�� ����A��
	string songTitle;
	if (!f->Get(FrameworkSetting::SongTitle, &songTitle))
		songTitle = "GirlTellMe";

	vector<SmInfo*>* sInfos = smManager->GetSmInfos();
	LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : there's [" << sInfos->size() << "] songs in sm manager.";

	for (int i = 0; i < sInfos->size(); i++) {

		LOG(LogLevel::Fine) << "--------------------------------- " << sInfos->at(i)->metadata->Title;

		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : song [" << songTitle << "] is [" << sInfos->at(i)->metadata->Title << "]?";

		//if (smInfos->at(i)->metadata->Title == songTitle) {
		if (sInfos->at(i)->metadata->Title == songTitle) { // ���ӭn�g�@��metedata�A���L�o���K�A���Ȯɥ�filename�N�n

			LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : song [" << songTitle << "] found.";

			SelectionChanged(sInfos->at(i));
			//delete smInfos; //�o�Ӳ{�b����delete�A�]���{�b�令�g���A�����^�ӱq�ɮ�Ū���ɭԤ~��delete


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

				frameworkConfigManager->Set(FrameworkSetting::TargetHeight, height); // ���O10�N�O15�A�������άO�¥��䤣�P��
			}
		}

		

	}


	// �o�@�q�n�b�}�l���ɤ��e�e�A�T�{�^�̭����S���o���q
	if (command->GetCommand() == MeteoCommand::SheetmusicData) {
		int songId = command->GetContext()["Id"].get<int>();

		vector<SmInfo*>* sInfos = smManager->GetSmInfos();
		for (int i = 0; i < sInfos->size(); i++) {
			if (sInfos->at(i)->id == songId) {
				// �^�Ǥw���o�����l


				return 0;
			}
			else {
				// �^�ǨS���o�����l
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
			//	// �U���q��...
			//
			//}

		}

		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++) {

			//if (inputState->GetBluetoothState()->GetCommands()->at(i)->CommandType == "Select") {
			//
			//	// ��ܺq��...
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
