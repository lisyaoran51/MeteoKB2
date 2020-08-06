#include "MeteoPacketConverterV1.h"


using namespace Desktop::Devices;


MeteoPacketConverterV1::MeteoPacketConverterV1(Storage* s)
{
	storage = s;

	commandMap[0x000000] = MeteoCommand::ReadFirmwareVersion;
	commandMap[0x110000] = MeteoCommand::ReturnFirmwareVersion;
	commandMap[0x000001] = MeteoCommand::ReadFirmwareData;
	commandMap[0x110001] = MeteoCommand::ReturnFirmwareData;
	commandMap[0x000002] = MeteoCommand::ReadHardwareData;
	commandMap[0x110002] = MeteoCommand::ReturnHardwareDate;
	commandMap[0x000003] = MeteoCommand::ReadHardwareIdentity;
	commandMap[0x110003] = MeteoCommand::ReturnHardwarIdentity;
	commandMap[0x100008] = MeteoCommand::ReadAppData;
	commandMap[0x010008] = MeteoCommand::ReturnAppData;
	commandMap[0x100009] = MeteoCommand::ReadUserData;
	commandMap[0x010009] = MeteoCommand::ReturnUserData;
	commandMap[0x00000A] = MeteoCommand::ChangeUserData;
	commandMap[0x11000A] = MeteoCommand::AckChangeUserData;

	commandMap[0x000100] = MeteoCommand::ReadMainboardStatus;
	commandMap[0x110100] = MeteoCommand::ReturnMainboardStatus;
	commandMap[0x000101] = MeteoCommand::RestartMainboard;
	commandMap[0x110101] = MeteoCommand::AckRestartMainboard;
	commandMap[0x000102] = MeteoCommand::RestartControlboard;
	commandMap[0x110102] = MeteoCommand::AckRestartControlboard;
	commandMap[0x100103] = MeteoCommand::RestartApp;
	commandMap[0x010103] = MeteoCommand::AckRestartApp;

	commandMap[0x000200] = MeteoCommand::SetHearbeatFrequency;
	commandMap[0x110200] = MeteoCommand::AckSetHearbeatFrequency;
	commandMap[0x000201] = MeteoCommand::Ping;
	commandMap[0x110201] = MeteoCommand::Pong;
	commandMap[0x100202] = MeteoCommand::HardwareRequestTerminate;
	commandMap[0x010202] = MeteoCommand::AckHardwareRequestTerminate;
	commandMap[0x000203] = MeteoCommand::AppRequestTerminate;
	commandMap[0x110203] = MeteoCommand::AckAppRequestTerminate;
	commandMap[0x100204] = MeteoCommand::ReadAppTime;
	commandMap[0x010204] = MeteoCommand::ReturnAppTime;
	
	commandMap[0x000300] = MeteoCommand::PressPowerButton;
	commandMap[0x110300] = MeteoCommand::AckPressPowerButton;
	commandMap[0x000301] = MeteoCommand::PressSustainButton;
	commandMap[0x110301] = MeteoCommand::AckPressSustainButton;
	commandMap[0x000302] = MeteoCommand::PressSensitiveButton;
	commandMap[0x110302] = MeteoCommand::AckPressSensitiveButton;
	commandMap[0x000303] = MeteoCommand::PressLowerOctaveButton;
	commandMap[0x110303] = MeteoCommand::AckPressLowerOctaveButton;
	commandMap[0x000304] = MeteoCommand::PressRaiseOctaveButton;
	commandMap[0x110304] = MeteoCommand::AckPressRaiseOctaveButton;
	commandMap[0x000305] = MeteoCommand::AdjustInstrumentVolume;
	commandMap[0x110305] = MeteoCommand::AckAdjustInstrumentVolume;
	commandMap[0x000306] = MeteoCommand::AdjustBackgroundButton;
	commandMap[0x110306] = MeteoCommand::AckAdjustBackgroundButton;
	commandMap[0x000307] = MeteoCommand::PluginPedal;
	commandMap[0x110307] = MeteoCommand::AckPluginPedal;
	commandMap[0x000308] = MeteoCommand::PluginEarphone;
	commandMap[0x110308] = MeteoCommand::AckPluginEarphone;

	commandMap[0x000400] = MeteoCommand::NewFirmwareData;
	commandMap[0x110400] = MeteoCommand::AckNewFirmwareData;
	commandMap[0x000401] = MeteoCommand::NewFirmwareFileSegment;
	commandMap[0x110401] = MeteoCommand::AckNewFirmwareFileSegment;
	commandMap[0x100402] = MeteoCommand::RequestRewriteNewFirmwareFileSegment;
	commandMap[0x000403] = MeteoCommand::RewriteNewFirmwareFileSegment;
	commandMap[0x110403] = MeteoCommand::AckRewriteNewFirmwareFileSegment;
	commandMap[0x000404] = MeteoCommand::FinishWriteNewFirmwareFile;
	commandMap[0x110404] = MeteoCommand::AckFinishWriteNewFirmwareFile;
	commandMap[0x000408] = MeteoCommand::NewInstrumentPackageData;
	commandMap[0x110408] = MeteoCommand::AckNewInstrumentPackageData;
	commandMap[0x000409] = MeteoCommand::NewInstrumentFileSegment;
	commandMap[0x110409] = MeteoCommand::AckNewInstrumentFileSegment;
	commandMap[0x00040A] = MeteoCommand::FinishWriteNewInstrumentFile;
	commandMap[0x11040A] = MeteoCommand::AckFinishWriteNewInstrumentFile;
	commandMap[0x00040B] = MeteoCommand::FinishWriteNewInstrumentFilePackage;
	commandMap[0x11040B] = MeteoCommand::AckFinishWriteNewInstrumentFilePackage;
	commandMap[0x10040C] = MeteoCommand::RequestRewriteNewInstrumentFile;
	commandMap[0x00040D] = MeteoCommand::RewriteNewInstrumentFile;
	commandMap[0x11040D] = MeteoCommand::AckRewriteNewInstrumentFile;
	commandMap[0x10040E] = MeteoCommand::RequestRewriteNewInstrumentFileSegment;
	commandMap[0x00040F] = MeteoCommand::RewriteNewInstrumentFileSegment;
	commandMap[0x11040F] = MeteoCommand::AckRewriteNewInstrumentFileSegment;

	commandMap[0x000500] = MeteoCommand::ChangeHardwareLogLevel;
	commandMap[0x110500] = MeteoCommand::AckChangeHardwareLogLevel;
	commandMap[0x000501] = MeteoCommand::DownloadHardwareLog;
	commandMap[0x100502] = MeteoCommand::HardwareLogFileSegment;
	commandMap[0x010502] = MeteoCommand::AckHardwareLogFileSegment;
	commandMap[0x000503] = MeteoCommand::DeleteHardwareLog;
	commandMap[0x110503] = MeteoCommand::AckDeleteHardwareLog;
	commandMap[0x000504] = MeteoCommand::SaveHardwareLog;
	commandMap[0x110504] = MeteoCommand::AckSaveHardwareLog;
	commandMap[0x000505] = MeteoCommand::SwitchInstantLog;
	commandMap[0x110505] = MeteoCommand::AckSwitchInstantLog;
	commandMap[0x110506] = MeteoCommand::InstantLog;
	commandMap[0x100507] = MeteoCommand::StopInstantLog;
	commandMap[0x010507] = MeteoCommand::AckStopInstantLog;

	commandMap[0x000600] = MeteoCommand::ReadHardwareConfiguration;
	commandMap[0x110600] = MeteoCommand::ReturnHardwareConfiguration;
	commandMap[0x000601] = MeteoCommand::WriteHardwareConfiguration;
	commandMap[0x110601] = MeteoCommand::ReturnWriteHardwareConfiguration;
	commandMap[0x000602] = MeteoCommand::ReadHardwareRecord;
	commandMap[0x110602] = MeteoCommand::ReturnHardwareRecord;

	commandMap[0x000700] = MeteoCommand::ReadScene;
	commandMap[0x110700] = MeteoCommand::ReturnScene;
	commandMap[0x000701] = MeteoCommand::ReadSceneAvailability;
	commandMap[0x110701] = MeteoCommand::ReturnSceneAvailability;
	commandMap[0x000702] = MeteoCommand::EnterScene;
	commandMap[0x110702] = MeteoCommand::AckEnterScene;
	commandMap[0x000703] = MeteoCommand::ReadSubsceneAvailability;
	commandMap[0x110703] = MeteoCommand::RerturnSubsceneAvailability;
	commandMap[0x000704] = MeteoCommand::EnterSubscene;
	commandMap[0x110704] = MeteoCommand::AckEnterSubscene;
	
	
	commandMap[0x000800] = MeteoCommand::ReadGameConfiguration;
	commandMap[0x110800] = MeteoCommand::ReturnGameConfiguration;
	commandMap[0x000801] = MeteoCommand::WriteGameConfiguration;
	commandMap[0x110801] = MeteoCommand::ReturnWriteGameConfiguration;
	commandMap[0x000802] = MeteoCommand::SheetmusicData;
	commandMap[0x110802] = MeteoCommand::AckSheetmusicData;
	commandMap[0x000803] = MeteoCommand::SheetmusicFileSegment;
	commandMap[0x110803] = MeteoCommand::AckSheetmusicFileSegment;
	commandMap[0x100804] = MeteoCommand::RequestRewriteSheetmusicFileSegment;
	commandMap[0x000805] = MeteoCommand::RewriteSheetmusicFileSegment;
	commandMap[0x110805] = MeteoCommand::AckRewriteSheetmusicFileSegment;
	commandMap[0x000806] = MeteoCommand::FinishWriteSheetmusic;
	commandMap[0x110806] = MeteoCommand::AckFinishWriteSheetmusic;
	commandMap[0x000807] = MeteoCommand::RequestLoadGame;
	commandMap[0x110807] = MeteoCommand::AckRequestLoadGame;
	commandMap[0x100808] = MeteoCommand::FinishLoadGame;
	commandMap[0x010808] = MeteoCommand::AckFinishLoadGame;
	
	commandMap[0x100810] = MeteoCommand::StartGame;
	commandMap[0x010810] = MeteoCommand::AckStartGame;
	commandMap[0x110811] = MeteoCommand::GameTime;
	commandMap[0x100812] = MeteoCommand::HardwareChangeGameTime;
	commandMap[0x010812] = MeteoCommand::AckHardwareChangeGameTime;
	commandMap[0x000813] = MeteoCommand::AppChangeGameTime;
	commandMap[0x110813] = MeteoCommand::AckAppChangeGameTime;
	commandMap[0x000814] = MeteoCommand::HardwareChangeGameSection;
	commandMap[0x110814] = MeteoCommand::AckHardwareChangeGameSection;
	commandMap[0x000815] = MeteoCommand::AppChangeGameSection;
	commandMap[0x110815] = MeteoCommand::AckAppChangeGameSection;
	commandMap[0x100816] = MeteoCommand::HardwarePauseGame;
	commandMap[0x010816] = MeteoCommand::AckHardwarePauseGame;
	commandMap[0x000817] = MeteoCommand::AppPauseGame;
	commandMap[0x110817] = MeteoCommand::AckAppPauseGame;
	commandMap[0x100818] = MeteoCommand::HardwareResumeGame;
	commandMap[0x010818] = MeteoCommand::AckHardwareResumeGame;
	commandMap[0x000819] = MeteoCommand::AppResumeGame;
	commandMap[0x110819] = MeteoCommand::AckAppResumeGame;
	commandMap[0x11081A] = MeteoCommand::PressKey;
	commandMap[0x11081B] = MeteoCommand::ReleaseKey;
	commandMap[0x11081C] = MeteoCommand::NowScore;
	commandMap[0x11081D] = MeteoCommand::HardwareSwitchRepeatPracticeState;
	commandMap[0x11081E] = MeteoCommand::HardwareSwitchRepeatPracticeRound;
	commandMap[0x10080F] = MeteoCommand::HardwareTerminateGame;
	commandMap[0x01081F] = MeteoCommand::AckHardwareTerminateGame;
	commandMap[0x000820] = MeteoCommand::AppTerminateGame;
	commandMap[0x110820] = MeteoCommand::AckAppTerminateGame;
	
	commandMap[0x100840] = MeteoCommand::HardwareFinishGame;
	commandMap[0x010840] = MeteoCommand::AckHardwareFinishGame;
	commandMap[0x100841] = MeteoCommand::FinalScore;
	commandMap[0x010841] = MeteoCommand::AckFinalScore;
	commandMap[0x100842] = MeteoCommand::PlayRecordData;
	commandMap[0x010842] = MeteoCommand::AckPlayRecordData;
	commandMap[0x000843] = MeteoCommand::PlayRecordFileSegment;
	commandMap[0x110843] = MeteoCommand::AckPlayRecordFileSegment;
	commandMap[0x100844] = MeteoCommand::RequestRewritePlayRecordFileSegment;
	commandMap[0x000845] = MeteoCommand::RewritePlayRecordFileSegment;
	commandMap[0x110845] = MeteoCommand::AckRewritePlayRecordFileSegment;
	commandMap[0x000846] = MeteoCommand::FinishWritePlayRecord;
	commandMap[0x110846] = MeteoCommand::AckFinishWritePlayRecord;

	SetJsonPacketTypeCommand(MeteoCommand::ReadFirmwareVersion);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnFirmwareVersion);
	SetJsonPacketTypeCommand(MeteoCommand::ReadFirmwareData);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnFirmwareData);
	SetJsonPacketTypeCommand(MeteoCommand::ReadHardwareData);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnHardwareDate);
	SetJsonPacketTypeCommand(MeteoCommand::ReadHardwareIdentity);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnHardwarIdentity);
	SetJsonPacketTypeCommand(MeteoCommand::ReadAppData);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnAppData);
	SetJsonPacketTypeCommand(MeteoCommand::ReadUserData);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnUserData);
	SetJsonPacketTypeCommand(MeteoCommand::ChangeUserData);
	SetJsonPacketTypeCommand(MeteoCommand::AckChangeUserData);

	SetJsonPacketTypeCommand(MeteoCommand::ReadMainboardStatus);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnMainboardStatus);
	SetJsonPacketTypeCommand(MeteoCommand::RestartMainboard);
	SetJsonPacketTypeCommand(MeteoCommand::AckRestartMainboard);
	SetJsonPacketTypeCommand(MeteoCommand::RestartControlboard);
	SetJsonPacketTypeCommand(MeteoCommand::AckRestartControlboard);
	SetJsonPacketTypeCommand(MeteoCommand::RestartApp);
	SetJsonPacketTypeCommand(MeteoCommand::AckRestartApp);

	SetJsonPacketTypeCommand(MeteoCommand::SetHearbeatFrequency);
	SetJsonPacketTypeCommand(MeteoCommand::AckSetHearbeatFrequency);
	SetJsonPacketTypeCommand(MeteoCommand::Ping);
	SetJsonPacketTypeCommand(MeteoCommand::Pong);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareRequestTerminate);
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwareRequestTerminate);
	SetJsonPacketTypeCommand(MeteoCommand::AppRequestTerminate);
	SetJsonPacketTypeCommand(MeteoCommand::AckAppRequestTerminate);
	SetJsonPacketTypeCommand(MeteoCommand::ReadAppTime);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnAppTime);

	SetJsonPacketTypeCommand(MeteoCommand::PressPowerButton);
	SetJsonPacketTypeCommand(MeteoCommand::AckPressPowerButton);
	SetJsonPacketTypeCommand(MeteoCommand::PressSustainButton);
	SetJsonPacketTypeCommand(MeteoCommand::AckPressSustainButton);
	SetJsonPacketTypeCommand(MeteoCommand::PressSensitiveButton);
	SetJsonPacketTypeCommand(MeteoCommand::AckPressSensitiveButton);
	SetJsonPacketTypeCommand(MeteoCommand::PressLowerOctaveButton);
	SetJsonPacketTypeCommand(MeteoCommand::AckPressLowerOctaveButton);
	SetJsonPacketTypeCommand(MeteoCommand::PressRaiseOctaveButton);
	SetJsonPacketTypeCommand(MeteoCommand::AckPressRaiseOctaveButton);
	SetJsonPacketTypeCommand(MeteoCommand::AdjustInstrumentVolume);
	SetJsonPacketTypeCommand(MeteoCommand::AckAdjustInstrumentVolume);
	SetJsonPacketTypeCommand(MeteoCommand::AdjustBackgroundButton);
	SetJsonPacketTypeCommand(MeteoCommand::AckAdjustBackgroundButton);
	SetJsonPacketTypeCommand(MeteoCommand::PluginPedal);
	SetJsonPacketTypeCommand(MeteoCommand::AckPluginPedal);
	SetJsonPacketTypeCommand(MeteoCommand::PluginEarphone);
	SetJsonPacketTypeCommand(MeteoCommand::AckPluginEarphone);

	SetJsonPacketTypeCommand(MeteoCommand::NewFirmwareData);
	SetJsonPacketTypeCommand(MeteoCommand::AckNewFirmwareData);
	SetJsonPacketTypeCommand(MeteoCommand::NewFirmwareFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::AckNewFirmwareFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteNewFirmwareFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::RewriteNewFirmwareFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::AckRewriteNewFirmwareFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWriteNewFirmwareFile);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWriteNewFirmwareFile);
	SetJsonPacketTypeCommand(MeteoCommand::NewInstrumentPackageData);
	SetJsonPacketTypeCommand(MeteoCommand::AckNewInstrumentPackageData);
	SetJsonPacketTypeCommand(MeteoCommand::NewInstrumentFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::AckNewInstrumentFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWriteNewInstrumentFile);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWriteNewInstrumentFile);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWriteNewInstrumentFilePackage);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWriteNewInstrumentFilePackage);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteNewInstrumentFile);
	SetJsonPacketTypeCommand(MeteoCommand::RewriteNewInstrumentFile);
	SetJsonPacketTypeCommand(MeteoCommand::AckRewriteNewInstrumentFile);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteNewInstrumentFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::RewriteNewInstrumentFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::AckRewriteNewInstrumentFileSegment);

	SetJsonPacketTypeCommand(MeteoCommand::ChangeHardwareLogLevel);
	SetJsonPacketTypeCommand(MeteoCommand::AckChangeHardwareLogLevel);
	SetJsonPacketTypeCommand(MeteoCommand::DownloadHardwareLog);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareLogFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwareLogFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::DeleteHardwareLog);
	SetJsonPacketTypeCommand(MeteoCommand::AckDeleteHardwareLog);
	SetJsonPacketTypeCommand(MeteoCommand::SaveHardwareLog);
	SetJsonPacketTypeCommand(MeteoCommand::AckSaveHardwareLog);
	SetJsonPacketTypeCommand(MeteoCommand::SwitchInstantLog);
	SetJsonPacketTypeCommand(MeteoCommand::AckSwitchInstantLog);
	SetJsonPacketTypeCommand(MeteoCommand::InstantLog);
	SetJsonPacketTypeCommand(MeteoCommand::StopInstantLog);
	SetJsonPacketTypeCommand(MeteoCommand::AckStopInstantLog);

	SetJsonPacketTypeCommand(MeteoCommand::ReadHardwareConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnHardwareConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::WriteHardwareConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnWriteHardwareConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::ReadHardwareRecord);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnHardwareRecord);

	SetJsonPacketTypeCommand(MeteoCommand::ReadScene);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnScene);
	SetJsonPacketTypeCommand(MeteoCommand::ReadSceneAvailability);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnSceneAvailability);
	SetJsonPacketTypeCommand(MeteoCommand::EnterScene);
	SetJsonPacketTypeCommand(MeteoCommand::AckEnterScene);
	SetJsonPacketTypeCommand(MeteoCommand::ReadSubsceneAvailability);
	SetJsonPacketTypeCommand(MeteoCommand::RerturnSubsceneAvailability);
	SetJsonPacketTypeCommand(MeteoCommand::EnterSubscene);
	SetJsonPacketTypeCommand(MeteoCommand::AckEnterSubscene);
	
	
	SetJsonPacketTypeCommand(MeteoCommand::ReadGameConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnGameConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::WriteGameConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnWriteGameConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::SheetmusicData);
	SetJsonPacketTypeCommand(MeteoCommand::AckSheetmusicData);
	SetJsonPacketTypeCommand(MeteoCommand::SheetmusicFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::AckSheetmusicFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteSheetmusicFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::RewriteSheetmusicFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::AckRewriteSheetmusicFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWriteSheetmusic);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWriteSheetmusic);
	SetJsonPacketTypeCommand(MeteoCommand::RequestLoadGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckRequestLoadGame);
	SetJsonPacketTypeCommand(MeteoCommand::FinishLoadGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishLoadGame);
	
	SetJsonPacketTypeCommand(MeteoCommand::StartGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckStartGame);
	SetJsonPacketTypeCommand(MeteoCommand::GameTime);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareChangeGameTime);
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwareChangeGameTime);
	SetJsonPacketTypeCommand(MeteoCommand::AppChangeGameTime);
	SetJsonPacketTypeCommand(MeteoCommand::AckAppChangeGameTime);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareChangeGameSection);
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwareChangeGameSection);
	SetJsonPacketTypeCommand(MeteoCommand::AppChangeGameSection);
	SetJsonPacketTypeCommand(MeteoCommand::AckAppChangeGameSection);
	SetJsonPacketTypeCommand(MeteoCommand::HardwarePauseGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwarePauseGame);
	SetJsonPacketTypeCommand(MeteoCommand::AppPauseGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPauseGame);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareResumeGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwareResumeGame);
	SetJsonPacketTypeCommand(MeteoCommand::AppResumeGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckAppResumeGame);
	SetJsonPacketTypeCommand(MeteoCommand::PressKey);
	SetJsonPacketTypeCommand(MeteoCommand::ReleaseKey);
	SetJsonPacketTypeCommand(MeteoCommand::NowScore);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareSwitchRepeatPracticeState);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareSwitchRepeatPracticeRound);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareTerminateGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwareTerminateGame);
	SetJsonPacketTypeCommand(MeteoCommand::AppTerminateGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckAppTerminateGame);
	
	SetJsonPacketTypeCommand(MeteoCommand::HardwareFinishGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwareFinishGame);
	SetJsonPacketTypeCommand(MeteoCommand::FinalScore);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinalScore);
	SetJsonPacketTypeCommand(MeteoCommand::PlayRecordData);
	SetJsonPacketTypeCommand(MeteoCommand::AckPlayRecordData);
	SetJsonPacketTypeCommand(MeteoCommand::PlayRecordFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::AckPlayRecordFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewritePlayRecordFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::RewritePlayRecordFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::AckRewritePlayRecordFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWritePlayRecord);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWritePlayRecord);

}

PacketType MeteoPacketConverterV1::CheckPacketType(char * buffer, int size)
{
	unsigned long command = 0x0;
	memcpy(&command, buffer, sizeof(command));

	map<unsigned long, MeteoCommand>::iterator iter;
	iter = commandMap.find(command);
	if (iter != commandMap.end()) {

		map<MeteoCommand, PacketType>::iterator iter2;
		iter2 = CommandPacketTypeMap.find(commandMap[command]);
		iter2 = CommandPacketTypeMap.find(commandMap[command]);
		if(iter2 != CommandPacketTypeMap.end())
			return CommandPacketTypeMap[commandMap[command]];
	}

	return PacketType::None;
}

BluetoothCommand * MeteoPacketConverterV1::ConvertToBluetoothCommand(char * buffer, int size)
{
	unsigned long command = 0x0;
	memcpy(&command, buffer, sizeof(command));

	map<unsigned long, MeteoCommand>::iterator iter;
	iter = commandMap.find(command);
	if (iter != commandMap.end()) {

		if (CheckPacketType(buffer, size) == PacketType::Json) {

			BluetoothCommand* btCommand = new MeteoBluetoothCommand(commandMap[command]);

			unsigned short length;
			memcpy(&length, buffer + sizeof(command), sizeof(length));
			
			memset(contextBuffer, 0, sizeof(contextBuffer));
			memcpy(contextBuffer,
				buffer + sizeof(command) + sizeof(length) + sizeof(unsigned short) * 2,
				length - (sizeof(command) + sizeof(length) + sizeof(unsigned short) * 2));
			// Id:unisgned int
			// Length:unsigned short 長度
			// Category : unsigned short 類別
			// Function : unsigned short 功能
			// Text : char[] 內文

			btCommand->GetContext() = json::parse(contextBuffer);

			return btCommand;
		}
	}

	return nullptr;
}

MeteoBluetoothCommand * MeteoPacketConverterV1::ConvertToBluetoothCommand(BluetoothMessage * bluetoothMessage)
{
	// TODO: 丟出去的部分還沒寫
	return nullptr;
}

int MeteoPacketConverterV1::ConvertToByteArray(BluetoothCommand * bluetoothCommand, char * buffer, int bufferMaxSize)
{
	// TODO: 丟出去的部分還沒寫
	return 0;
}

MeteoBluetoothCommand * MeteoPacketConverterV1::FinishWriteFile(BluetoothCommand * bluetoothCommand)
{
	if (!dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand))
		return nullptr;

	if (dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand() == MeteoCommand::FinishWriteSheetmusic) {
		string fileName = dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetContext()["FileName"];

		MeteoPacketConverterFileSegmentMap* file = fileMap[fileName];

		if (writeFile(file, string("Songs")) > -1) {
			delete file;

			fileMap.erase(fileName);

			//return returnBluetoothCommand;
		}
		else {
			// 回傳需重傳的片段
			//return returnBluetoothCommand;
		}
		

	}

	return nullptr;
}

bool MeteoPacketConverterV1::CheckIsWrtieFileFinishCommand(BluetoothCommand * bluetoothCommand)
{
	return false;
}

MeteoBluetoothCommand* MeteoPacketConverterV1::ConvertToFile(char * buffer, int size)
{
	unsigned long command = 0x0;
	memcpy(&command, buffer, sizeof(command));

	map<unsigned long, MeteoCommand>::iterator iter;
	iter = commandMap.find(command);
	if (iter != commandMap.end()) {

		if (CheckPacketType(buffer, size) == PacketType::File) {

			string fileName = getFileName(buffer, size);
			int fileSegmentSize = getFileSize(buffer, size);
			char* fileSegment = getFileSegment(buffer, size);
			int fileSegmentNumber = getFileSegmentNumber(buffer, size);
			MeteoPacketConverterFileType fileType = getFileType(buffer, size);

			MeteoPacketConverterFileSegmentMap* fileSegmentMap = nullptr;

			map<string, MeteoPacketConverterFileSegmentMap*>::iterator iter2;
			iter2 = fileMap.find(fileName);
			if (iter2 != fileMap.end()) {
				fileSegmentMap = fileMap[fileName];
			}
			else {
				fileSegmentMap = new MeteoPacketConverterFileSegmentMap();
				fileSegmentMap->fileName = fileName;
				fileSegmentMap->segmentAmount = fileSegmentSize;
				fileSegmentMap->fileType = fileType;
				fileMap[fileName] = fileSegmentMap;
			}

			fileSegmentMap->fileSegmentMap[fileSegmentNumber] = pair<char*, int>(fileSegment, fileSegmentSize);
			// TODO:檢查重複


			return nullptr;
		}
	}

	return nullptr;
}

bool MeteoPacketConverterV1::CheckIsFinishWriteCommand(BluetoothCommand * bluetoothCommand)
{
	return false;
}

int MeteoPacketConverterV1::writeFile(MeteoPacketConverterFileSegmentMap * file, string path)
{
	if (file->fileSegmentMap.size() < file->segmentAmount)
		return -1;

	fstream* fileStream = storage->GetStream(path + string("/") + file->fileName, false, true);

	for (int i = 0; i < file->fileSegmentMap.size(); i++) {

		fileStream->write(file->fileSegmentMap[i].first, file->fileSegmentMap[i].second);

	}

	fileStream->close();

	delete fileStream;

	return 0;
}
