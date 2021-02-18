#include "SheetmusicSelectPanel.h"

#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../IO/Communications/GetBinaryBleRequest.h"


using namespace Games::UI;
using namespace Games::Output::Bluetooths;
using namespace Games::IO::Communications;



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

	isPresent = true;

	smManager = s;

	frameworkConfigManager = f;

	outputManager = o;

	communicationAccess = c;

	selectedModifiers.SetValue(new vector<Modifier*>());

	//return 0;

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

int SheetmusicSelectPanel::OnMessage(MeteoBluetoothMessage * message)
{
	MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);

	if (contextMessage == nullptr) {
		return -1;
	}
	json context = contextMessage->GetContextInJson();

	LOG(LogLevel::Debug) << "SheetmusicSelectPanel::OnMessage() : got new bt message. ";

	if (contextMessage->GetCommand() == MeteoCommand::WriteHardwareConfiguration) {
		for (int i = 0; i < context["Configurations"].size(); i++) {
			if (context["Configurations"].at(i)["category"].get<string>() == "Framework" &&
				context["Configurations"].at(i)["Object"].get<string>() == "TargetHeight") {
				int height = context["Configurations"].at(i)["Value"].get<int>();

				frameworkConfigManager->Set(FrameworkSetting::TargetHeight, height); // 不是10就是15，對齊黑鍵或是黑白鍵不同高
			}
		}
	}

	if (contextMessage->GetCommand() == MeteoCommand::WriteGameConfiguration) {
		vector<string> modifierIds;
		string modifierName;
		int value1, value2;
		for (int i = 0; i < context["Modifiers"].size(); i++) {
			// TODO: 下面這行寫錯了，之後要再改
			modifierName = context["Modifiers"].at(i)["Modifier Name"].get<string>() == "AutoPedalModifier";
			value1 = context["Modifiers"].at(i)["Value1"].get<int>();
			value2 = context["Modifiers"].at(i)["Value2"].get<int>();

			InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
				
			LOG(LogLevel::Fine) << "SheetmusicSelectPanel::OnMessage() : select autopedal modifier";

			Modifier* modifier = iCreator.CreateInstanceWithT<Modifier>(modifierName);
			modifier->SetValue(value1, value2);

			if (AddModifier(modifier))
				modifierIds.push_back(context["Modifiers"].at(i)["Modifier Id"].get<string>());

		}

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::ReturnWriteGameConfiguration);
		
		json returnContext;

		returnContext["Scene"] = string("SongSelect");
		for (int i = 0; i < modifierIds.size(); i++) {
			json modifierId;
			modifierId["Modifier Id"] = modifierIds[i];
			returnContext["Status"].push_back(modifierId);
		}

		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);
	}

	/* 寫入譜檔片段 */
	if (message->GetCommand() == MeteoCommand::SheetmusicFileSegment) {



	}

	// 這一段要在開始傳檔之前送，確認琴裡面有沒有這首歌
	if (message->GetCommand() == MeteoCommand::SheetmusicData) {
		int songId = context["Id"].get<int>();
		string fileName = context["FileName"].get<string>();

		vector<SmInfo*>* sInfos = smManager->GetSmInfos();
		for (int i = 0; i < sInfos->size(); i++) {
			if (sInfos->at(i)->id == songId) {
				// 回傳已有這首曲子
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
		// 回傳沒有這首曲子
		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckSheetmusicData);
		json returnContext;

		returnContext["Status"] = short(0);
		returnContext["FileName"] = fileName;
		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);

		/* 開始下載樂譜reuqest */
		MeteoContextBluetoothMessage* getSheetmusicMessage = new MeteoContextBluetoothMessage(MeteoCommand::RequestSheetmusicFile);
		json requestContext;

		requestContext["FileName"] = fileName;
		getSheetmusicMessage->SetContextInJson(requestContext);
		getSheetmusicMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		GetBinaryBleRequest* getSheetmusicRequest = new GetBinaryBleRequest(string("Sheetmusics/") + fileName,
			getSheetmusicMessage,
			MeteoCommand::AckRequestSheetmusicFile,
			MeteoCommand::SheetmusicFileSegment,
			MeteoCommand::FinishWriteSheetmusic,
			MeteoCommand::RequestRewriteSheetmusicFileSegment,
			MeteoCommand::AckFinishWriteSheetmusic);

		getSheetmusicRequest->SetCallbackScene(dynamic_cast<Scene*>(GetParent()));	// 這個的parent就是song select

		getSheetmusicRequest->AddOnFinish(&onDownloadSheetmusicFinish);
		getSheetmusicRequest->AddOnSuccess(&onGetSheetmusicSuccess);

		communicationAccess->Queue(getSheetmusicRequest);

		return 0;
	}

	if (message->GetCommand() == MeteoCommand::RequestLoadGame) {

		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::OnMessage() : request load game. " << context.dump();

		string fileName = context["FileName"].get<string>();
		

		LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::OnMessage() : selected file name [" << fileName << "].";

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckRequestLoadGame);
		json returnContext;

		returnContext["Status"] = short(0);
		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);

		vector<SmInfo*>* sInfos = smManager->GetSmInfos();
		for (int i = 0; i < sInfos->size(); i++) {
			LOG(LogLevel::Fine) << "int SheetmusicSelectPanel::OnMessage() : searching song name [" << sInfos->at(i)->fileName << "] = [" << fileName << "].";
			if (sInfos->at(i)->fileName == fileName) {

				SelectionChanged(sInfos->at(i));

				StartRequest();
				return 0;
			}
		}
		LOG(LogLevel::Error) << "int SheetmusicSelectPanel::OnMessage() : song not found.";
	}


	return -1;
}

int SheetmusicSelectPanel::onMessage(InputState * inputState, InputKey command)
{

	return 0;
}
