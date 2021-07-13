#include "SheetmusicSelectPanel.h"

#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../IO/Communications/GetBinaryBleRequest.h"
#include "../../Framework/Threading/ThreadMaster.h"


using namespace Games::UI;
using namespace Games::Output::Bluetooths;
using namespace Games::IO::Communications;
using namespace Framework::Threading;




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

	CommunicationAccess * c = GetCache<CommunicationAccess>("CommunicationAccess");
	if (!c)
		throw runtime_error("int SheetmusicSelectPanel::load : CommunicationAccess not found in cache.");


	return load(f, s, o, c);
}

int SheetmusicSelectPanel::load(FrameworkConfigManager * f, SmManager * s, OutputManager* o, CommunicationAccess* c)
{
	LOG(LogLevel::Info) << "int SheetmusicSelectPanel::load() : insert sheetmusic manager.";
	LOG(LogLevel::Depricated) << "int SheetmusicSelectPanel::load() : message active" << isMessageActive;

	isPresent = true;

	smManager = s;

	frameworkConfigManager = f;

	outputManager = o;

	communicationAccess = c;

	selectedModifiers.SetValue(new vector<Modifier*>());

	return 0;

	// �Ȯɥ��o�� ����A��
	string songTitle;
	if (!f->Get(FrameworkSetting::SongTitle, &songTitle))
		songTitle = "Canon";

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

BindablePointer<vector<Modifier*>*>* SheetmusicSelectPanel::GetSelectedModifiers()
{
	return &selectedModifiers;
}

int SheetmusicSelectPanel::AddModifier(Modifier * modifier)
{
	// TODO: �ˬd���S���Ĭ�modifier�A�M���Ĭ�modifier�R��
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

int SheetmusicSelectPanel::SelectAndStart(string fileName)
{
	LOG(LogLevel::Error) << "int SheetmusicSelectPanel::SelectAndStart() : not implemented.";
	return 0;
}

int SheetmusicSelectPanel::AddOnDownloadSheetmusicSuccess(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name)
{
	onGetSheetmusicSuccess.Add(callableObject, callback, name);
	return 0;
}

int SheetmusicSelectPanel::AddOnDownloadSheetmusicFail(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name)
{
	onGetSheetmusicFail.Add(callableObject, callback, name);
	return 0;
}

int SheetmusicSelectPanel::onMessage(MeteoBluetoothMessage * message)
{
	MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);

	if (contextMessage == nullptr) {
		return -1;
	}
	json context = contextMessage->GetContextInJson();

	LOG(LogLevel::Debug) << "SheetmusicSelectPanel::onMessage() : got new bt message. ";

	

	if (contextMessage->GetCommand() == MeteoCommand::ClearGameConfiguration) {
		LOG(LogLevel::Debug) << "SheetmusicSelectPanel::onMessage() : clear modifiers. ";
		
		for (int i = 0; i < selectedModifiers.GetValue()->size(); i++) {

			delete selectedModifiers.GetValue()->at(i);

		}
		selectedModifiers.GetValue()->clear();

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckClearGameConfiguration);

		json returnContext;

		returnContext["status"] = 0;

		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);
	}

	if (contextMessage->GetCommand() == MeteoCommand::WriteGameConfiguration) {
		string modifierName;
		float value1, value2;
		for (int i = 0; i < context["Modifiers"].size(); i++) {
			// TODO: �U���o��g���F�A����n�A��
			modifierName = context["Modifiers"].at(i)["Name"].get<string>();// == "AutoPedalModifier";
			value1 = context["Modifiers"].at(i)["V1"].get<float>();
			value2 = context["Modifiers"].at(i)["V2"].get<float>();

			InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
			
			LOG(LogLevel::Debug) << "SheetmusicSelectPanel::onMessage() : select" << modifierName << "with value [" << value1 << "],[" << "].";

			Modifier* modifier = iCreator.CreateInstanceWithT<Modifier>(modifierName);
			modifier->SetValue(value1, value2);

			AddModifier(modifier);

		}


		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckWriteGameConfiguration);
		
		json returnContext;

		returnContext["Status"] = 0;

		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);
	}

	// �o�@�q�n�b�}�l���ɤ��e�e�A�T�{�^�̭����S���o���q
	if (message->GetCommand() == MeteoCommand::SheetmusicData) {
		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::onMessage() : get message SheetmusicData";

		try {
			string fileName = context["FileName"].get<string>();

			vector<SmInfo*>* sInfos = smManager->GetSmInfos();
			for (int i = 0; i < sInfos->size(); i++) {
				LOG(LogLevel::Depricated) << "int SheetmusicSelectPanel::onMessage() : check song name " << sInfos->at(i)->fileName;

				if (sInfos->at(i)->fileName == fileName) {
					LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::onMessage() : has song " << sInfos->at(i)->fileName;
					// �^�Ǥw���o�����l
					MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckSheetmusicData);

					json returnContext;
					returnContext["Status"] = short(-1);
					returnContext["FileName"] = fileName;
					meteoContextBluetoothMessage->SetContextInJson(returnContext);
					meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

					outputManager->PushMessage(meteoContextBluetoothMessage);
					return 0;
				}
			}

			/*
			 * ���n
			 */
			ThreadMaster::GetInstance().SwitchGameStatus((int)GameStatus::Download);
			/*
			 * ���n
			 */

			LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::onMessage() : not have song " << fileName;

			// �^�ǨS���o�����l
			MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckSheetmusicData);
			json returnContext;

			returnContext["Status"] = short(0);
			returnContext["FileName"] = fileName;
			meteoContextBluetoothMessage->SetContextInJson(returnContext);
			meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

			outputManager->PushMessage(meteoContextBluetoothMessage);

			/* �}�l�U������reuqest */
			MeteoContextBluetoothMessage* getSheetmusicMessage = new MeteoContextBluetoothMessage(MeteoCommand::RequestSheetmusicFile);
			json requestContext;

			requestContext["FileName"] = fileName;
			getSheetmusicMessage->SetContextInJson(requestContext);
			getSheetmusicMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

			GetBinaryBleRequest* getSheetmusicRequest = new GetBinaryBleRequest(string("/home/pi/Sheetmusics/") + fileName,
				getSheetmusicMessage,
				MeteoCommand::AckRequestSheetmusicFile,
				MeteoCommand::SheetmusicFileSegment,
				MeteoCommand::AckSheetmusicFileSegment,
				MeteoCommand::FinishWriteSheetmusic,
				MeteoCommand::RequestRewriteSheetmusicFileSegment,
				MeteoCommand::AckFinishWriteSheetmusic);

			getSheetmusicRequest->SetCallbackScene(dynamic_cast<Scene*>(GetParent()));	// �o�Ӫ�parent�N�Osong select

			getSheetmusicRequest->AddOnSuccess(&onGetSheetmusicSuccess);
			getSheetmusicRequest->AddOnFail(&onGetSheetmusicFail);

			LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::onMessage() : start queuing " << fileName;

			GetScheduler()->AddDelayedTask([=]() {

				LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::onMessage() : queue request to get " << fileName;

				communicationAccess->Queue(getSheetmusicRequest);

				return 0;
			}, 0.1);

			return 0;
		}
		catch (exception& e) {
			LOG(LogLevel::Error) << "SheetmusicSelectPanel::onMessage() : " << e.what();

			return 0;
		}
	}

	if (message->GetCommand() == MeteoCommand::RequestLoadGame) {

		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::onMessage() : request load game. ";

		string fileName = context["FileName"].get<string>();
		

		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::onMessage() : selected file name [" << fileName << "].";

		

		vector<SmInfo*>* sInfos = smManager->GetSmInfos();
		for (int i = 0; i < sInfos->size(); i++) {
			LOG(LogLevel::Fine) << "int SheetmusicSelectPanel::onMessage() : searching song name [" << sInfos->at(i)->fileName << "] = [" << fileName << "].";
			if (sInfos->at(i)->fileName == fileName) {

				MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckRequestLoadGame);
				json returnContext;

				returnContext["Status"] = short(0);
				meteoContextBluetoothMessage->SetContextInJson(returnContext);
				meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

				outputManager->PushMessage(meteoContextBluetoothMessage);


				/*
				 * ���n
				 */
				ThreadMaster::GetInstance().SwitchGameStatus((int)GameStatus::Game);
				/*
				 * ���n
				 */

				SelectionChanged(sInfos->at(i));

				StartRequest();
				return 0;
			}
		}
		LOG(LogLevel::Error) << "int SheetmusicSelectPanel::onMessage() : song not found.";

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckRequestLoadGame);
		json returnContext;

		returnContext["Status"] = short(-1);
		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);

	}


	return -1;
}

int SheetmusicSelectPanel::onMessage(InputState * inputState, InputKey command)
{

	return 0;
}
