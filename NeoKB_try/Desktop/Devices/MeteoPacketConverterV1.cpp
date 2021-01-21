#include "MeteoPacketConverterV1.h"

#include "../../Util/Log.h"
//#include "../../Games/Output/Bluetooths/Commands/MeteoOutputFileBluetoothCommand.h"
//#include "../../Games/Input/Commands/MeteoAckFileBluetoothCommand.h"


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
	// TODO: ����n�[��L�ɮ�����
	return MeteoPacketConverterFileType::Sheetmusic;
}

MeteoBluetoothCommand * MeteoPacketConverterV1::createAckFileSegmentBluetoothCommand(char * buffer, int size)
{
	// �ھڥ�L�Ӫ�packet�إߦ^��
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
	// �ѧO
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
	// �t��
	commandMap[0x000100] = MeteoCommand::ReadMainboardStatus;
	commandMap[0x110100] = MeteoCommand::ReturnMainboardStatus;
	commandMap[0x000101] = MeteoCommand::RestartMainboard;
	commandMap[0x110101] = MeteoCommand::AckRestartMainboard;
	commandMap[0x000102] = MeteoCommand::RestartControlboard;
	commandMap[0x110102] = MeteoCommand::AckRestartControlboard;
	commandMap[0x100103] = MeteoCommand::RestartApp;
	commandMap[0x010103] = MeteoCommand::AckRestartApp;
	// �߸�
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
	// �򥻥\��
	commandMap[0x000300] = MeteoCommand::AppPressPowerButton			   ;
	commandMap[0x110300] = MeteoCommand::AckAppPressPowerButton			   ;
	commandMap[0x100300] = MeteoCommand::PianoPressPowerButton			   ;
	commandMap[0x010300] = MeteoCommand::AckPianoPressPowerButton		   ;
	commandMap[0x000301] = MeteoCommand::AppPressSustainButton			   ;
	commandMap[0x110301] = MeteoCommand::AckAppPressSustainButton		   ;
	commandMap[0x100301] = MeteoCommand::PianoPressSustainButton		   ;
	commandMap[0x010301] = MeteoCommand::AckPianoPressSustainButton		   ;
	commandMap[0x000302] = MeteoCommand::AppPressSensitiveButton		   ;
	commandMap[0x110302] = MeteoCommand::AckAppPressSensitiveButton		   ;
	commandMap[0x100302] = MeteoCommand::PianoPressSensitiveButton		   ;
	commandMap[0x010302] = MeteoCommand::AckPianoPressSensitiveButton	   ;
	commandMap[0x000303] = MeteoCommand::AppPressLowerOctaveButton		   ;
	commandMap[0x110303] = MeteoCommand::AckAppPressLowerOctaveButton	   ;
	commandMap[0x100303] = MeteoCommand::PianoPressLowerOctaveButton	   ;
	commandMap[0x010303] = MeteoCommand::AckPianoPressLowerOctaveButton	   ;
	commandMap[0x000304] = MeteoCommand::AppPressRaiseOctaveButton		   ;
	commandMap[0x110305] = MeteoCommand::AckAppPressRaiseOctaveButton	   ;
	commandMap[0x100304] = MeteoCommand::PianoPressRaiseOctaveButton	   ;
	commandMap[0x010305] = MeteoCommand::AckPianoPressRaiseOctaveButton	   ;
	commandMap[0x000305] = MeteoCommand::AppPressPauseButton			   ;
	commandMap[0x110305] = MeteoCommand::AckAppPressPauseButton			   ;
	commandMap[0x100305] = MeteoCommand::PianoPressPauseButton			   ;
	commandMap[0x010305] = MeteoCommand::AckPianoPressPauseButton		   ;
	commandMap[0x000306] = MeteoCommand::AppAdjustInstrumentVolume		   ;
	commandMap[0x110306] = MeteoCommand::AckAppAdjustInstrumentVolume	   ;
	commandMap[0x100306] = MeteoCommand::PianoAdjustInstrumentVolume	   ;
	commandMap[0x010306] = MeteoCommand::AckPianoAdjustInstrumentVolume	   ;
	commandMap[0x000307] = MeteoCommand::AppAdjustBackgroundButton		   ;
	commandMap[0x110307] = MeteoCommand::AckAppAdjustBackgroundButton	   ;
	commandMap[0x100307] = MeteoCommand::PianoAdjustBackgroundButton	   ;
	commandMap[0x010307] = MeteoCommand::AckPianoAdjustBackgroundButton	   ;
	commandMap[0x000308] = MeteoCommand::AppPluginPedal					   ;
	commandMap[0x110308] = MeteoCommand::AckAppPluginPedal				   ;
	commandMap[0x100308] = MeteoCommand::PianoPluginPedal				   ;
	commandMap[0x010308] = MeteoCommand::AckPianoPluginPedal			   ;
	commandMap[0x000309] = MeteoCommand::AppPluginEarphone				   ;
	commandMap[0x110309] = MeteoCommand::AckAppPluginEarphone			   ;
	commandMap[0x100309] = MeteoCommand::PianoPluginEarphone			   ;
	commandMap[0x010309] = MeteoCommand::AckPianoPluginEarphone			   ;
	commandMap[0x00030A] = MeteoCommand::AppPressKey					   ;
	commandMap[0x11030A] = MeteoCommand::AckAppPressKey					   ;
	commandMap[0x10030A] = MeteoCommand::PianoPressKey					   ;
	commandMap[0x01030A] = MeteoCommand::AckPianoPressKey				   ;
	commandMap[0x00030B] = MeteoCommand::AppReleaseKey					   ;
	commandMap[0x11030B] = MeteoCommand::AckAppReleaseKey				   ;
	commandMap[0x10030B] = MeteoCommand::PianoReleaseKey				   ;
	commandMap[0x01030B] = MeteoCommand::AckPianoReleaseKey				   ;
	commandMap[0x00030C] = MeteoCommand::AppPushInSustainPedal			   ;
	commandMap[0x11030C] = MeteoCommand::AckAppPushInSustainPedal		   ;
	commandMap[0x10030C] = MeteoCommand::PianoPushInSustainPedal		   ;
	commandMap[0x01030C] = MeteoCommand::AckPianoPushInSustainPedal		   ;
	commandMap[0x00030D] = MeteoCommand::AppReleaseSustainPedal			   ;
	commandMap[0x11030D] = MeteoCommand::AckAppReleaseSustainPedal		   ;
	commandMap[0x10030D] = MeteoCommand::PianoReleaseSustainPedal		   ;
	commandMap[0x01030D] = MeteoCommand::AckPianoReleaseSustainPedal	   ;
	commandMap[0x000310] = MeteoCommand::AppReadPianoInstruments		   ;
	commandMap[0x110310] = MeteoCommand::AckAppReadPianoInstruments		   ;
	commandMap[0x100310] = MeteoCommand::PianoReadAppInstruments		   ;
	commandMap[0x010310] = MeteoCommand::AckPianoReadAppInstruments		   ;
	commandMap[0x000311] = MeteoCommand::AppSwitchInstrument			   ;
	commandMap[0x110311] = MeteoCommand::AckAppSwitchInstrument			   ;
	commandMap[0x100311] = MeteoCommand::PianoSwitchInstrument			   ;
	commandMap[0x010311] = MeteoCommand::AckPianoSwitchInstrument		   ;
	commandMap[0x000312] = MeteoCommand::FinishAppSwitchInstrument		   ;
	commandMap[0x110312] = MeteoCommand::AckFinishAppSwitchInstrument	   ;
	commandMap[0x100312] = MeteoCommand::FinishPianoSwitchInstrument	   ;
	commandMap[0x010312] = MeteoCommand::AckFinishPianoSwitchInstrument	   ;
	commandMap[0x000313] = MeteoCommand::AppSwitchPianoInstrument		   ;
	commandMap[0x110313] = MeteoCommand::AckAppSwitchPianoInstrument	   ;
	commandMap[0x100314] = MeteoCommand::FinishAppSwitchPianoInstrument	   ;
	commandMap[0x010314] = MeteoCommand::AckFinishAppSwitchPianoInstrument ;
	// �X�R�ɯ�
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
	// log
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
	// �]�w
	commandMap[0x000600] = MeteoCommand::ReadHardwareConfiguration;
	commandMap[0x110600] = MeteoCommand::ReturnHardwareConfiguration;
	commandMap[0x000601] = MeteoCommand::WriteHardwareConfiguration;
	commandMap[0x110601] = MeteoCommand::ReturnWriteHardwareConfiguration;
	commandMap[0x000602] = MeteoCommand::ReadHardwareRecord;
	commandMap[0x110602] = MeteoCommand::ReturnHardwareRecord;
	// ��ĵ
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
	
	// ���ֹC��
	// �C���ǳ�
	
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
	// �C���i��
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
	commandMap[0x000820] = MeteoCommand::AppQuitGame;
	commandMap[0x110820] = MeteoCommand::AckAppQuitGame;
	commandMap[0x000821] = MeteoCommand::AppRestartGame;
	commandMap[0x110821] = MeteoCommand::AckAppRestartGame;

	// �C������
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

	
	// �ѧO
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
	// �t��
	SetJsonPacketTypeCommand(MeteoCommand::ReadMainboardStatus);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnMainboardStatus);
	SetJsonPacketTypeCommand(MeteoCommand::RestartMainboard);
	SetJsonPacketTypeCommand(MeteoCommand::AckRestartMainboard);
	SetJsonPacketTypeCommand(MeteoCommand::RestartControlboard);
	SetJsonPacketTypeCommand(MeteoCommand::AckRestartControlboard);
	SetJsonPacketTypeCommand(MeteoCommand::RestartApp);
	SetJsonPacketTypeCommand(MeteoCommand::AckRestartApp);
	// �߸�
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
	// �򥻥\��
	SetJsonPacketTypeCommand(MeteoCommand::AppPressPowerButton			  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPressPowerButton			 ); 
	SetJsonPacketTypeCommand(MeteoCommand::PianoPressPowerButton			 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoPressPowerButton		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AppPressSustainButton			 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPressSustainButton		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::PianoPressSustainButton		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoPressSustainButton		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AppPressSensitiveButton		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPressSensitiveButton		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::PianoPressSensitiveButton		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoPressSensitiveButton	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AppPressLowerOctaveButton		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPressLowerOctaveButton	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::PianoPressLowerOctaveButton	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoPressLowerOctaveButton	 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AppPressRaiseOctaveButton		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPressRaiseOctaveButton	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::PianoPressRaiseOctaveButton	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoPressRaiseOctaveButton	 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AppPressPauseButton			  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPressPauseButton			 ); 
	SetJsonPacketTypeCommand(MeteoCommand::PianoPressPauseButton			 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoPressPauseButton		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AppAdjustInstrumentVolume		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppAdjustInstrumentVolume	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::PianoAdjustInstrumentVolume	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoAdjustInstrumentVolume	 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AppAdjustBackgroundButton		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppAdjustBackgroundButton	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::PianoAdjustBackgroundButton	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoAdjustBackgroundButton	 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AppPluginPedal					 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPluginPedal				 ); 
	SetJsonPacketTypeCommand(MeteoCommand::PianoPluginPedal				  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoPluginPedal			  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AppPluginEarphone				 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPluginEarphone			  	 );
	SetJsonPacketTypeCommand(MeteoCommand::PianoPluginEarphone			  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoPluginEarphone			 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AppPressKey					  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPressKey					 ); 
	SetJsonPacketTypeCommand(MeteoCommand::PianoPressKey					 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoPressKey				  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AppReleaseKey					 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppReleaseKey				  	 );
	SetJsonPacketTypeCommand(MeteoCommand::PianoReleaseKey				  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoReleaseKey				 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AppPushInSustainPedal			 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPushInSustainPedal		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::PianoPushInSustainPedal		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoPushInSustainPedal		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AppReleaseSustainPedal			 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppReleaseSustainPedal		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::PianoReleaseSustainPedal		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoReleaseSustainPedal	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AppReadPianoInstruments		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckAppReadPianoInstruments		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::PianoReadAppInstruments		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoReadAppInstruments		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AppSwitchInstrument			  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckAppSwitchInstrument			 ); 
	SetJsonPacketTypeCommand(MeteoCommand::PianoSwitchInstrument			 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckPianoSwitchInstrument		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::FinishAppSwitchInstrument		 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishAppSwitchInstrument	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::FinishPianoSwitchInstrument	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishPianoSwitchInstrument	 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AppSwitchPianoInstrument		  	 );
	SetJsonPacketTypeCommand(MeteoCommand::AckAppSwitchPianoInstrument	  	 );
	SetJsonPacketTypeCommand(MeteoCommand::FinishAppSwitchPianoInstrument	 ); 
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishAppSwitchPianoInstrument );

	// �X�R�ɯ�
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
	// log
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
	// �]�w
	SetJsonPacketTypeCommand(MeteoCommand::ReadHardwareConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnHardwareConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::WriteHardwareConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnWriteHardwareConfiguration);
	SetJsonPacketTypeCommand(MeteoCommand::ReadHardwareRecord);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnHardwareRecord);
	// ��ĵ
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
	// ���ֹC��
	// �C���ǳ�
	
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
	// �C���i��
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
	SetJsonPacketTypeCommand(MeteoCommand::AppQuitGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckAppQuitGame);
	SetJsonPacketTypeCommand(MeteoCommand::AppRestartGame);
	SetJsonPacketTypeCommand(MeteoCommand::AckAppRestartGame);
	// �C������
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

	/* �p�G���ʥ]��n�bbuffer�̧��ڳQ���_�A�N��true */
	bool isPacketCut = false;

	/* �N�e�@��read��buffer�̫�@�ӳQ���_���ʥ]��o����read��buffer append�b�@�_(�p�G�e�@��read�S���Q���_���ʥ]�A�N����) */
	char* appendedBufferIn = nullptr;
	int appendedBytesRead = 0;

	/* �p�G�W�����ʥ]���ɭԡA���_��packet�A�N����{�b�o��buffer�W */
	if (lastBufferSegmentSize > 0) {
		
		memcpy(lastBufferSegment + lastBufferSegmentSize, bufferIn, bytesRead);
		appendedBufferIn = lastBufferSegment;
		appendedBytesRead = lastBufferSegmentSize + bytesRead;

	}
	/* �p�G�W�����ʥ]�S���_����packet�A�N�����β{�b��buffer�N�n�A���ΦAappend */
	else {
		appendedBufferIn = bufferIn;
		appendedBytesRead = bytesRead;
	}

	/* �����C�ӫʥ]���_�I */
	char* splitPosition = appendedBufferIn;

	while (totalByteSplited < appendedBytesRead) {

		unsigned long command;
		memcpy(&command, splitPosition, sizeof(command));
		unsigned short length = 0;
		memcpy(&length, splitPosition + sizeof(command), sizeof(length));

		LOG(LogLevel::Debug) << "MeteoPacketConverterV1::SplitPacket() : get packet [" << hex << command << dec << "], length [" << length << "].";

		/* �P�_�ʥ]�O�_�l�a�C�̤j�ʥ]���׬�538�A�p�G�W�L�N�N��w�l�a */
		/* �ʥ]�L�u�A�i��]�w�g����a���F */
		if (length > maxPacketLength || 
			(length < 10 && length != 8 && length != 4)) {

			char* tempPacket = splitPosition;
			returnPackets.push_back(splitPosition);
			returnPacketLengths.push_back(appendedBytesRead - totalByteSplited);
			totalByteSplited = appendedBytesRead;
			LOG(LogLevel::Debug) << "MeteoPacketConverterV1::SplitPacket() : get broken packet with length [" << length << "].";
			continue;
		}
		/* �P�_�ʥ]�O�_�W�L�ثeŪ�쪺��ƪ��סA�W�L���ܥi��bread���ɭԳQ���_�F�A�N�n�⥦���lastBufferSegment�̭��Ȧs�_�� */
		else if (length > appendedBytesRead - totalByteSplited) {

			memset(lastBufferSegment, 0, sizeof(char) * 2048);
			memcpy(lastBufferSegment, splitPosition, appendedBytesRead - totalByteSplited);
			lastBufferSegmentSize = appendedBytesRead - totalByteSplited;
			totalByteSplited = appendedBytesRead;
			isPacketCut = true;
			LOG(LogLevel::Debug) << "MeteoPacketConverterV1::SplitPacket() : get last packet cut with length [" << lastBufferSegmentSize << "] / [" << length << "].";

		}
		/* ���` */
		else {

			char* tempPacket = splitPosition;
			returnPackets.push_back(tempPacket);
			returnPacketLengths.push_back(length);
			splitPosition += length;
			totalByteSplited += length;
		}

		
	}

	if (!isPacketCut)
		lastBufferSegmentSize = 0;


	/* �ˬd���쪺packets���S���W�Lbuffer�̤j�q128�� */
	if (returnPackets.size() > 128) {
		LOG(LogLevel::Error) << "MeteoPacketConverterV1::SplitPacket() : get packet amount overflow [" << returnPackets.size() << "].";


	}

	/* ����n��packet�ƻs�@����i�^�ǭȸ� */
	for (int i = 0; i < returnPackets.size(); i++) {

		packets[i] = new char[returnPacketLengths[i]];
		memcpy(packets[i], returnPackets[i], returnPacketLengths[i]);

		packerLengths[i] = returnPacketLengths[i];
	}

	return returnPackets.size();
}

PacketStatus MeteoPacketConverterV1::CheckPacketStatus(char * packet, int length)
{

	LOG(LogLevel::Debug) << "MeteoPacketConverterV1::CheckPacketStatus() : packet [" << length << "].";

	unsigned long command;
	memcpy(&command, packet, sizeof(command));
	unsigned short packetLength = 0;
	memcpy(&packetLength, packet + sizeof(command), sizeof(packetLength));


	LOG(LogLevel::Debug) << "MeteoPacketConverterV1::CheckPacketStatus() : command [" << hex << command << dec << "], length[" << packetLength << "].";

	/* �P�_�ʥ]�O�_�l�a�C�̤j�ʥ]���׬�538�A�p�G�W�L�N�N��w�l�a */
	/* �P�_�ʥ]�O�_�W�L�ثeŪ�쪺��ƪ��סA�W�L���ܥi��bread���ɭԳQ���_�F */
	/* �ʥ]�L�u�A�i��]�w�g����a���F */
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

PacketType MeteoPacketConverterV1::CheckCommandType(BluetoothMessage * bluetoothCommand)
{
	MeteoCommand command = dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand();
	
	map<MeteoCommand, PacketType>::iterator iter;
	iter = CommandPacketTypeMap.find(command);
	if (iter != CommandPacketTypeMap.end())
		return CommandPacketTypeMap[command];

	return PacketType::None;
}

BluetoothMessage * MeteoPacketConverterV1::ConvertToBluetoothMessage(char * buffer, int size)
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
			// Length:unsigned short ����(2)
			// Order : unsigned short ����(2)
			// Amount : unsigned short �ʥ]��(2)
			// Text : char[] ����

			btCommand->GetContext() = json::parse(contextBuffer);

			LOG(LogLevel::Debug) << "MeteoPacketConverterV1::ConvertToBluetoothCommand() : command [" << hex << command << dec << "], context [" << btCommand->GetContext().dump() << "].";

			// TODO: parse���ѭnerror handle

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

int MeteoPacketConverterV1::GetCountOfPacket(BluetoothMessage * bluetoothCommand)
{
	if (dynamic_cast<MeteoOutputFileBluetoothCommand*>(bluetoothCommand)) {
		return dynamic_cast<MeteoOutputFileBluetoothCommand*>(bluetoothCommand)->GetFileSegmentCount();
	}
	else if (dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)) {
		// TODO: �ȮɳW�w�Ҧ�command�j�p����W�L538�A�N�O�@��packet���̤j�ؤo
		return 1;
	}

	return -1;
}

int MeteoPacketConverterV1::ConvertToByteArray(BluetoothMessage * bluetoothCommand, int packetOrder, char * buffer, int bufferMaxSize)
{
	// �令�g�bbluetooth phone�̭��F


	// TODO: ��X�h�������٨S�g
	return 0;
}

BluetoothMessage * MeteoPacketConverterV1::FinishWriteFile(BluetoothMessage * bluetoothCommand)
{
	if (!dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand))
		return nullptr;


	if (dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand() == MeteoCommand::FinishWriteSheetmusic) {
		string fileName = dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetContext()["FileName"];

		// �������ɦW
		string documentName = fileName.substr(0, fileName.find(string("."), 0));

		MeteoPacketConverterFileSegmentMap* file = fileMap[fileName];

		// sm�ɦ�m��"song/�ɦW/�ɦW.sm"
		if (writeFile(file, string("Songs/") + documentName) > -1) {
			delete file;

			fileMap.erase(fileName);

			MeteoBluetoothCommand* returnBluetoothCommand = new MeteoBluetoothCommand(MeteoCommand::AckFinishWriteSheetmusic);
			returnBluetoothCommand->GetContext()["FileName"] = fileName;

			return returnBluetoothCommand;

		}
		else {

			/* �إ߰T���i�D����S���쪺Segment�O�X�� */
			MeteoBluetoothCommand* returnBluetoothCommand = new MeteoBluetoothCommand(MeteoCommand::RequestRewriteSheetmusicFileSegment);
			returnBluetoothCommand->GetContext()["FileName"] = fileName;

			/* �ˬd���X��segment�S������ */
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

bool MeteoPacketConverterV1::CheckIsWrtieFileFinishCommand(BluetoothMessage * bluetoothCommand)
{
	if (dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand() == MeteoCommand::AckFinishWriteNewFirmwareFile ||
		dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand() == MeteoCommand::AckFinishWriteNewInstrumentFile ||
		dynamic_cast<MeteoBluetoothCommand*>(bluetoothCommand)->GetCommand() == MeteoCommand::AckFinishWriteSheetmusic) {
		return true;
	}

	return false;
}

BluetoothMessage* MeteoPacketConverterV1::ConvertToFile(char * buffer, int size)
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

			/* �p�G�����ƪ�file segment�A�N���ª��R���ɤW�s�� */
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

bool MeteoPacketConverterV1::CheckIsFinishWriteCommand(BluetoothMessage * bluetoothCommand)
{
	return false;
}

int MeteoPacketConverterV1::writeFile(MeteoPacketConverterFileSegmentMap * file, string path)
{
	if (file->fileSegmentMap.size() < file->segmentAmount)
		return -1;

	//fstream* fileStream = storage->GetStream(path + string("/") + file->fileName, true, true);	// �o�椣��ΡA���ΤU������

	fstream* fileStream = storage->GetStream(path + string("/") + file->fileName, FileAccess::Write, FileMode::Create, true);

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
