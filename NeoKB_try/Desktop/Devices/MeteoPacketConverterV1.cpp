#include "MeteoPacketConverterV1.h"

#include "../../Util/Log.h"
#include "../../Games/Output/Bluetooths/Commands/MeteoOutputFileBluetoothCommand.h"
#include "../../Games/Input/Commands/MeteoAckFileBluetoothCommand.h"


using namespace Desktop::Devices;
using namespace Util;
using namespace Games::Output::Bluetooths::Commands;
using namespace Games::Input::Commands;


string MeteoPacketConverterV1::getFileName(char * buffer, int size)
{

	char name[17] = { 0 };
	memcpy(name, buffer + sizeof(unsigned long) + sizeof(unsigned short), sizeof(char) * 16);

	for (int i = 0; i < 17; i++) {
		if (name[i] == 0x0) {
			name[i] = '\0';
			break;
		}
	}

	return string(name);
}

int MeteoPacketConverterV1::getFileSize(char * buffer, int size)
{
	unsigned short length;
	memcpy(&length, buffer + sizeof(unsigned long), sizeof(unsigned short));

	int fileSize = length - sizeof(unsigned long) + sizeof(unsigned short) + sizeof(char) * 16 + sizeof(unsigned short) * 2;

	return fileSize;
}

char * MeteoPacketConverterV1::getFileSegment(char * buffer, int size)
{
	int fileSegmentSize = getFileSize(buffer, size);

	if (fileSegmentSize < 0 && fileSegmentSize > maxFileSegmentSize) {
		LOG(LogLevel::Error) << "MeteoPacketConverterV1::getFileSegment() : wrong file segment size [" << fileSegmentSize << "].";
		return nullptr;
	}

	char* fileSegment = new char[fileSegmentSize];

	memcpy(fileSegment, 
		   buffer + sizeof(unsigned long) + sizeof(unsigned short) + sizeof(char) * 16 + sizeof(unsigned short) * 2, 
		   sizeof(char) * fileSegmentSize);



	return fileSegment;
}

int MeteoPacketConverterV1::getFileSegmentOrder(char * buffer, int size)
{
	unsigned short fileSegmentNumber;

	memcpy(&fileSegmentNumber, buffer + sizeof(unsigned long) + sizeof(unsigned short) + sizeof(char) * 16, sizeof(unsigned short));

	return fileSegmentNumber;
}

int MeteoPacketConverterV1::getFileSegmentCount(char * buffer, int size)
{
	unsigned short fileSegmentCount;

	memcpy(&fileSegmentCount, buffer + sizeof(unsigned long) + sizeof(unsigned short) + sizeof(char) * 16 + sizeof(unsigned short), sizeof(unsigned short));

	return fileSegmentCount;
}

MeteoPacketConverterV1::MeteoPacketConverterFileType MeteoPacketConverterV1::getFileType(char * buffer, int size)
{
	// TODO: 之後要加其他檔案類型
	return MeteoPacketConverterFileType::Sheetmusic;
}

MeteoBluetoothCommand * MeteoPacketConverterV1::createAckFileSegmentBluetoothCommand(char * buffer, int size)
{
	// 根據丟過來的packet建立回傳
	MeteoBluetoothCommand* returnBluetoothCommand = new MeteoBluetoothCommand(MeteoCommand::AckSheetmusicFileSegment);

	string fileName = getFileName(buffer, size);
	int order = getFileSegmentOrder(buffer, size);
	returnBluetoothCommand->GetContext()["FileName"] = fileName;
	returnBluetoothCommand->GetContext()["Order"] = order;

	return returnBluetoothCommand;
}

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

	SetReadFirmwareVersionPacketTypeCommand(MeteoCommand::ReadFirmwareVersion);
	SetReturnFirmwareVersionPacketTypeCommand(MeteoCommand::ReturnFirmwareVersion);
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
	SetFilePacketTypeCommand(MeteoCommand::NewFirmwareFileSegment);
	SetAckFilePacketTypeCommand(MeteoCommand::AckNewFirmwareFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteNewFirmwareFileSegment);
	SetFilePacketTypeCommand(MeteoCommand::RewriteNewFirmwareFileSegment);
	SetAckFilePacketTypeCommand(MeteoCommand::AckRewriteNewFirmwareFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWriteNewFirmwareFile);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWriteNewFirmwareFile);
	SetJsonPacketTypeCommand(MeteoCommand::NewInstrumentPackageData);
	SetJsonPacketTypeCommand(MeteoCommand::AckNewInstrumentPackageData);
	SetFilePacketTypeCommand(MeteoCommand::NewInstrumentFileSegment);
	SetAckFilePacketTypeCommand(MeteoCommand::AckNewInstrumentFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWriteNewInstrumentFile);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWriteNewInstrumentFile);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWriteNewInstrumentFilePackage);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWriteNewInstrumentFilePackage);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteNewInstrumentFile);
	SetJsonPacketTypeCommand(MeteoCommand::RewriteNewInstrumentFile);
	SetJsonPacketTypeCommand(MeteoCommand::AckRewriteNewInstrumentFile);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteNewInstrumentFileSegment);
	SetFilePacketTypeCommand(MeteoCommand::RewriteNewInstrumentFileSegment);
	SetAckFilePacketTypeCommand(MeteoCommand::AckRewriteNewInstrumentFileSegment);

	SetJsonPacketTypeCommand(MeteoCommand::ChangeHardwareLogLevel);
	SetJsonPacketTypeCommand(MeteoCommand::AckChangeHardwareLogLevel);
	SetJsonPacketTypeCommand(MeteoCommand::DownloadHardwareLog);
	SetFilePacketTypeCommand(MeteoCommand::HardwareLogFileSegment);
	SetAckFilePacketTypeCommand(MeteoCommand::AckHardwareLogFileSegment);
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
	SetFilePacketTypeCommand(MeteoCommand::SheetmusicFileSegment);
	SetAckFilePacketTypeCommand(MeteoCommand::AckSheetmusicFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteSheetmusicFileSegment);
	SetFilePacketTypeCommand(MeteoCommand::RewriteSheetmusicFileSegment);
	SetAckFilePacketTypeCommand(MeteoCommand::AckRewriteSheetmusicFileSegment);
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
	SetFilePacketTypeCommand(MeteoCommand::PlayRecordFileSegment);
	SetAckFilePacketTypeCommand(MeteoCommand::AckPlayRecordFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewritePlayRecordFileSegment);
	SetFilePacketTypeCommand(MeteoCommand::RewritePlayRecordFileSegment);
	SetAckFilePacketTypeCommand(MeteoCommand::AckRewritePlayRecordFileSegment);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWritePlayRecord);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWritePlayRecord);

}

int MeteoPacketConverterV1::SplitPacket(char * bufferIn, int bytesRead, char ** packets, int * packerLengths)
{
	int totalByteSplited = 0;

	vector<char*> returnPackets;
	vector<int> returnPacketLengths;

	char* splitPosition = bufferIn;

	while (totalByteSplited < bytesRead) {

		unsigned long command;
		unsigned short length = 0;
		memcpy(&length, splitPosition + sizeof(command), sizeof(length));

		/* 判斷封包是否損壞。最大封包長度為538，如果超過就代表已損壞 */
		/* 判斷封包是否超過目前讀到的資料長度，超過的話可能在read的時候被切斷了 */
		/* 封包過短，可能也已經整個壞掉了 */
		if (length > maxPacketLength || 
			length > bytesRead - totalByteSplited ||
			(length < 10 && length != 8 && length != 4)) {

			char* tempPacket = splitPosition;
			returnPackets.push_back(splitPosition);
			returnPacketLengths.push_back(bytesRead - totalByteSplited);
			totalByteSplited = bytesRead;
			continue;
		}

		char* tempPacket = splitPosition;
		returnPackets.push_back(tempPacket);
		returnPacketLengths.push_back(length);
		splitPosition += length;
		totalByteSplited += length;

	}

	/* 把切好的packet複製一份丟進回傳值裡 */
	packets = new char*[returnPackets.size()];
	packerLengths = new int[returnPackets.size()];
	for (int i = 0; i < returnPackets.size(); i++) {

		packets[i] = new char[returnPacketLengths[i]];
		memcpy(packets[i], returnPackets[i], returnPacketLengths[i]);

		packerLengths[i] = returnPacketLengths[i];
	}

	return returnPackets.size();
}

PacketStatus MeteoPacketConverterV1::CheckPacketStatus(char * packet, int length)
{

	unsigned long command;
	unsigned short packetLength = 0;
	memcpy(&packetLength, packet + sizeof(command), sizeof(packetLength));

	/* 判斷封包是否損壞。最大封包長度為538，如果超過就代表已損壞 */
	/* 判斷封包是否超過目前讀到的資料長度，超過的話可能在read的時候被切斷了 */
	/* 封包過短，可能也已經整個壞掉了 */
	if (packetLength > maxPacketLength) {
		return PacketStatus::OutOfBound;
	}

	if (packetLength > length) {
		return PacketStatus::Overlength;
	}

	if (packetLength < length) {
		return PacketStatus::Underlength;
	}

	if (packetLength < 10 && packetLength != 8 && packetLength != 4) {
		return PacketStatus::WrongLength;
	}

	map<unsigned long, MeteoCommand>::iterator iter;
	iter = commandMap.find(command);
	if (iter == commandMap.end()) {
		return PacketStatus::WrongCommand;
	}

	// TODO: wrong json format

	// TODO: damaged

	return PacketStatus::Fine;
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
		if(iter2 != CommandPacketTypeMap.end())
			return CommandPacketTypeMap[commandMap[command]];
	}

	return PacketType::None;
}

PacketType Desktop::Devices::MeteoPacketConverterV1::CheckCommandType(BluetoothCommand * bluetoothCommand)
{
	MeteoCommand command = dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand();
	
	map<MeteoCommand, PacketType>::iterator iter;
	iter = CommandPacketTypeMap.find(command);
	if (iter != CommandPacketTypeMap.end())
		return CommandPacketTypeMap[command];

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
			
			memset(contextBuffer, 0, sizeof(char) * 1024);
			memcpy(contextBuffer,
				buffer + sizeof(command) + sizeof(length) + sizeof(unsigned short) * 2,
				length - (sizeof(command) + sizeof(length) + sizeof(unsigned short) * 2));
			// Id:unisgned int(4)
			// Length:unsigned short 長度(2)
			// Order : unsigned short 順序(2)
			// Amount : unsigned short 封包數(2)
			// Text : char[] 內文

			btCommand->GetContext() = json::parse(contextBuffer);

			// TODO: parse失敗要error handle

			return btCommand;
		}
		else if (CheckPacketType(buffer, size) == PacketType::AckFile) {

			MeteoAckFileBluetoothCommand* btCommand = new MeteoAckFileBluetoothCommand(commandMap[command]);

			unsigned short length;
			memcpy(&length, buffer + sizeof(command), sizeof(length));

			string fileName = getFileName(buffer, size);

			int order = getFileSegmentOrder(buffer, size);

			btCommand->SetFileName(fileName);
			btCommand->SetOrder(order);

			return btCommand;

		}
	}

	return nullptr;
}

MeteoBluetoothCommand * MeteoPacketConverterV1::ConvertToBluetoothCommand(BluetoothMessage * bluetoothMessage)
{
	// TODO: 丟出去的部分還沒寫
	// 好像不用血這段



	return nullptr;
}

int MeteoPacketConverterV1::GetCountOfPacket(BluetoothCommand * bluetoothCommand)
{
	if (dynamic_cast<MeteoOutputFileBluetoothCommand*>(bluetoothCommand)) {
		return dynamic_cast<MeteoOutputFileBluetoothCommand*>(bluetoothCommand)->GetFileSegmentCount();
	}
	else if (dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)) {
		// TODO: 暫時規定所有command大小不能超過538，就是一個packet的最大尺寸
		return 1;
	}

	return -1;
}

int MeteoPacketConverterV1::ConvertToByteArray(BluetoothCommand * bluetoothCommand, int packetOrder, char * buffer, int bufferMaxSize)
{
	// 改成寫在bluetooth phone裡面了


	// TODO: 丟出去的部分還沒寫
	return 0;
}

BluetoothCommand * MeteoPacketConverterV1::FinishWriteFile(BluetoothCommand * bluetoothCommand)
{
	if (!dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand))
		return nullptr;


	if (dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand() == MeteoCommand::FinishWriteSheetmusic) {
		string fileName = dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetContext()["FileName"];

		// 拿掉附檔名
		string documentName = fileName.substr(0, fileName.find(string("."), 0));

		MeteoPacketConverterFileSegmentMap* file = fileMap[fileName];

		// sm檔位置為"song/檔名/檔名.sm"
		if (writeFile(file, string("Songs/") + documentName) > -1) {
			delete file;

			fileMap.erase(fileName);

			MeteoBluetoothCommand* returnBluetoothCommand = new MeteoBluetoothCommand(MeteoCommand::AckFinishWriteSheetmusic);
			returnBluetoothCommand->GetContext()["FileName"] = fileName;

			return returnBluetoothCommand;

		}
		else {

			/* 建立訊息告訴手機沒收到的Segment是幾號 */
			MeteoBluetoothCommand* returnBluetoothCommand = new MeteoBluetoothCommand(MeteoCommand::RequestRewriteSheetmusicFileSegment);
			returnBluetoothCommand->GetContext()["FileName"] = fileName;

			/* 檢查哪幾個segment沒有收到 */
			map<int, pair<char*, int>>::iterator iter;
			for (int i = 0; i < file->segmentAmount; i++) {
				iter = file->fileSegmentMap.find(i);
				if (iter == file->fileSegmentMap.end()) {

					if (returnBluetoothCommand->GetContext()["Orders"].size() > 15) {
						LOG(LogLevel::Fine) << "MeteoPacketConverterV1::FinishWriteFile() : sending rewrite reuqest overflow.";
						continue;
					}

					json order;
					order["Order"] = i;
					returnBluetoothCommand->GetContext()["Orders"].push_back(order);

				}
			}

			return returnBluetoothCommand;
		}
	}

	return nullptr;
}

bool MeteoPacketConverterV1::CheckIsWrtieFileFinishCommand(BluetoothCommand * bluetoothCommand)
{
	if (dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand() == MeteoCommand::AckFinishWriteNewFirmwareFile ||
		dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand() == MeteoCommand::AckFinishWriteNewInstrumentFile ||
		dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand() == MeteoCommand::AckFinishWriteSheetmusic) {
		return true;
	}

	return false;
}

MeteoBluetoothCommand* MeteoPacketConverterV1::ConvertToFile(char * buffer, int size)
{
	unsigned long command = 0x0;
	memcpy(&command, buffer, sizeof(command));

	map<unsigned long, MeteoCommand>::iterator iter;
	iter = commandMap.find(command);
	if (iter != commandMap.end()) {

		LOG(LogLevel::Fine) << "MeteoPacketConverterV1::ConvertToFile() : converting [" << command << "] command file.";

		if (CheckPacketType(buffer, size) == PacketType::File) {

			string fileName = getFileName(buffer, size);
			int fileSegmentSize = getFileSize(buffer, size);

			if (fileSegmentSize < 0 || fileSegmentSize > maxFileSegmentSize) {
				LOG(LogLevel::Error) << "MeteoPacketConverterV1::ConvertToFile() : wrong file size [" << fileSegmentSize << "].";
				return nullptr;
			}

			char* fileSegment = getFileSegment(buffer, size);
			int fileSegmentNumber = getFileSegmentOrder(buffer, size);
			int fileSegmentCount = getFileSegmentCount(buffer, size);
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
				fileSegmentMap->segmentAmount = fileSegmentCount;
				fileSegmentMap->fileType = fileType;
				fileMap[fileName] = fileSegmentMap;
			}

			/* 如果有重複的file segment，就把舊的刪掉補上新的 */
			map<int, pair<char*, int>>::iterator iter3;
			iter3 = fileSegmentMap->fileSegmentMap.find(fileSegmentNumber);
			if (iter3 != fileSegmentMap->fileSegmentMap.end()) {
				LOG(LogLevel::Error) << "MeteoPacketConverterV1::ConvertToFile() : duplicated file segment [" << fileSegmentNumber << "].";
				delete fileSegmentMap->fileSegmentMap[fileSegmentNumber].first;
			}

			fileSegmentMap->fileSegmentMap[fileSegmentNumber] = pair<char*, int>(fileSegment, fileSegmentSize);
			
			return createAckFileSegmentBluetoothCommand(buffer, size);
		}
	}

	LOG(LogLevel::Error) << "MeteoPacketConverterV1::ConvertToFile() : convert failed .";
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

	fstream* fileStream = storage->GetStream(path + string("/") + file->fileName, true, true);

	for (int i = 0; i < file->fileSegmentMap.size(); i++) {

		fileStream->write(file->fileSegmentMap[i].first, file->fileSegmentMap[i].second);

	}

	fileStream->close();

	delete fileStream;

	return 0;
}



MeteoPacketConverterV1::MeteoPacketConverterFileSegmentMap::~MeteoPacketConverterFileSegmentMap()
{
	for (int i = 0; i < fileSegmentMap.size(); i++) {
		delete[] fileSegmentMap[i].first;
	}


}
