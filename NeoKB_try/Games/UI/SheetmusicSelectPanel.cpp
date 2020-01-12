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

	// �Ȯɥ��o�� ����A��
	string songTitle;
	if (!f->Get(FrameworkSetting::SongTitle, &songTitle))
		songTitle = "Casual_Simple";

	vector<SmInfo*>* smInfos = smManager->GetSmInfos();
	LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : there's [" << smInfos->size() << "] songs in sm manager.";

	for (int i = 0; i < smInfos->size(); i++) {

		LOG(LogLevel::Fine) << "--------------------------------- " << smInfos->at(i)->metadata->Title;

		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : song [" << songTitle << "] is [" << smInfos->at(i)->metadata->Title << "]?";

		//if (smInfos->at(i)->metadata->Title == songTitle) {
		if (smInfos->at(i)->metadata->Title == songTitle) { // ���ӭn�g�@��metedata�A���L�o���K�A���Ȯɥ�filename�N�n

			LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : song [" << songTitle << "] found.";

			SelectionChanged(smInfos->at(i));
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

int SheetmusicSelectPanel::onCommand(InputState * inputState, InputKey command)
{

	if (command == InputKey::Bluetooth) {
		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++) {

			if (inputState->GetBluetoothState()->GetCommands()->at(i)->CommandType == "Download") {

				// �U���q��...

			}

		}

		for (int i = 0; i < inputState->GetBluetoothState()->GetCommands()->size(); i++) {

			if (inputState->GetBluetoothState()->GetCommands()->at(i)->CommandType == "Select") {

				// ��ܺq��...
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
