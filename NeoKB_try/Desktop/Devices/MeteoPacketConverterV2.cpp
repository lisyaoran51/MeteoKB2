#include "MeteoPacketConverterV2.h"

#include "../../Util/Log.h"
#include "../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Games/Output/Bluetooths/MeteoFileSegmentBluetoothMessage.h"
#include "../../Games/Output/Bluetooths/MeteoAckFileSegmentBluetoothMessage.h"


#define DEBUG_VARIANT


using namespace Desktop::Devices;
using namespace Util;
using namespace Games::Output::Bluetooths;
using namespace Games::Input::Commands;


string MeteoPacketConverterV2::getFileName(const char * buffer, int size)
{

	char name[17] = { 0 };
	memcpy(name, buffer + sizeof(unsigned int) + sizeof(unsigned short) * 2, sizeof(char) * 16);
	return string(name);

	for (int i = 0; i < 17; i++) {
		if (name[i] == 0x0) {
			name[i] = '\0';
			break;
		}
	}

	return string(name);
}

int MeteoPacketConverterV2::getFileSize(const char * buffer, int size)
{
	unsigned short length;
	memcpy(&length, buffer + sizeof(unsigned int) + sizeof(unsigned short), sizeof(unsigned short));

	int fileSize = length - sizeof(unsigned int) - sizeof(unsigned short) * 2 - sizeof(char) * 16 - sizeof(unsigned short) * 2;

	return fileSize;
}

char * MeteoPacketConverterV2::getFileSegment(const char * buffer, int size)
{
	int fileSegmentSize = getFileSize(buffer, size);

	if (fileSegmentSize < 0 && fileSegmentSize > maxFileSegmentSize) {
		LOG(LogLevel::Error) << "MeteoPacketConverterV1::getFileSegment() : wrong file segment size [" << fileSegmentSize << "].";
		return nullptr;
	}

	char* fileSegment = new char[fileSegmentSize];

	memcpy(fileSegment, 
		   buffer + sizeof(unsigned int) + sizeof(unsigned short) * 2 + sizeof(char) * 16 + sizeof(unsigned short) * 2, 
		   sizeof(char) * fileSegmentSize);



	return fileSegment;
}

int MeteoPacketConverterV2::getFileSegmentOrder(const char * buffer, int size)
{
	unsigned short fileSegmentNumber;

	memcpy(&fileSegmentNumber, buffer + sizeof(unsigned int) + sizeof(unsigned short) * 2 + sizeof(char) * 16, sizeof(unsigned short));

	return fileSegmentNumber;
}

int MeteoPacketConverterV2::getFileSegmentCount(const char * buffer, int size)
{
	unsigned short fileSegmentCount;

	memcpy(&fileSegmentCount, buffer + sizeof(unsigned int) + sizeof(unsigned short) * 2 + sizeof(char) * 16 + sizeof(unsigned short), sizeof(unsigned short));

	return fileSegmentCount;
}

MeteoPacketConverterV2::MeteoPacketConverterV2()
{
	// 識別
	commandMap[0x00000000] = MeteoCommand::ReadFirmwareVersion				;
	commandMap[0x01140000] = MeteoCommand::ReturnFirmwareVersion			;
	commandMap[0x02000001] = MeteoCommand::ReadFirmwareData					;
	commandMap[0x02120001] = MeteoCommand::ReturnFirmwareData				;
	commandMap[0x02000002] = MeteoCommand::ReadHardwareData					;
	commandMap[0x02140002] = MeteoCommand::ReturnHardwareData				;
	commandMap[0x02100003] = MeteoCommand::SendEncryptedIdentifyMessage		;
	commandMap[0x02040003] = MeteoCommand::ReturnDecryptedIdentifyMessage	;
	commandMap[0x02110004] = MeteoCommand::FaultIdentity					;
	commandMap[0x02200005] = MeteoCommand::TestMtu							;
	commandMap[0x02240005] = MeteoCommand::ReturnTestMtu					;
	commandMap[0x02100006] = MeteoCommand::ReadAppData						;
	commandMap[0x02040006] = MeteoCommand::ReturnAppData					;
	commandMap[0x02100007] = MeteoCommand::ReadUserData						;
	commandMap[0x02040007] = MeteoCommand::ReturnUserData					;
	commandMap[0x02000008] = MeteoCommand::ChangeUserData					;
	commandMap[0x02120008] = MeteoCommand::AckChangeUserData				;
	// 系統
	commandMap[0x02000101] = MeteoCommand::RestartMainboard				;	
	commandMap[0x02120101] = MeteoCommand::AckRestartMainboard			;	
	commandMap[0x02000102] = MeteoCommand::RestartControlboard			;	
	commandMap[0x02120102] = MeteoCommand::AckRestartControlboard		;
	commandMap[0x02100103] = MeteoCommand::RestartApp					;
	commandMap[0x20020103] = MeteoCommand::AckRestartApp				;
	commandMap[0x02000104] = MeteoCommand::RestartKeyboardProgram		;
	commandMap[0x02120104] = MeteoCommand::AckRestartKeyboardProgram	;
	commandMap[0x02110105] = MeteoCommand::KeyboardErrorMessage			;

	// 心跳
	commandMap[0x02000200] = MeteoCommand::SetPingPongFrequency		;
	commandMap[0x02120200] = MeteoCommand::AckSetPingPongFrequency	;
	commandMap[0x02010201] = MeteoCommand::Ping						;
	commandMap[0x02110201] = MeteoCommand::Pong						;
	commandMap[0x02100202] = MeteoCommand::ReadAppTime				;
	commandMap[0x02040202] = MeteoCommand::ReturnAppTime			;
	// 基本功能
	commandMap[0x02010300] = MeteoCommand::AppIOEvent						;					
	commandMap[0x02110300] = MeteoCommand::KeyboardIOEvent					;	
	commandMap[0x02000301] = MeteoCommand::AppReadKeyboardInstrument		;	
	commandMap[0x02140301] = MeteoCommand::ReturnAppReadKeyboardInstrument	;	
	commandMap[0x02000302] = MeteoCommand::AppSwitchKeyboardInstrument		;	
	commandMap[0x02120302] = MeteoCommand::AckAppSwitchKeyboardInstrument	;	
	commandMap[0x02100303] = MeteoCommand::KeyboardFinishSwitchInstrument	;	
	commandMap[0x02020303] = MeteoCommand::AckKeyboardFinishSwitchInstrument;	
	// 擴充升級
	commandMap[0x02000400] = MeteoCommand::NewFirmwareData							;				
	commandMap[0x02120400] = MeteoCommand::AckNewFirmwareData						;	
	commandMap[0x02000401] = MeteoCommand::ClearNewFirmwareSplit					;	
	commandMap[0x02120401] = MeteoCommand::AckClearNewFirmwareSplit					;
	commandMap[0x02100402] = MeteoCommand::RequestNewFirmwareSplit					;	
	commandMap[0x02020402] = MeteoCommand::AckRequestNewFirmwareSplit				;	
	commandMap[0x03000403] = MeteoCommand::NewFirmwareSplitFileSegment				;	
	commandMap[0x04120403] = MeteoCommand::AckNewFirmwareSplitFileSegment			;	
	commandMap[0x02110404] = MeteoCommand::RequestRewriteNewFirmwareSplitFileSegment;	
	commandMap[0x02000405] = MeteoCommand::FinishWriteNewFirmwareSplit				;	
	commandMap[0x02120405] = MeteoCommand::AckFinishWriteNewFirmwareSplit			;	
	commandMap[0x02100406] = MeteoCommand::FinishRequestNewFirmware					;
	commandMap[0x02020406] = MeteoCommand::AckFinishRequestNewFirmware				;	
	// log
	commandMap[0x02000500] = MeteoCommand::ChangeHardwareLogLevel					;	
	commandMap[0x02120500] = MeteoCommand::AckChangeHardwareLogLevel				;
	commandMap[0x02000501] = MeteoCommand::DownloadHardwareLog						;
	commandMap[0x02120501] = MeteoCommand::AckDownloadHardwareLog					;
	commandMap[0x03100502] = MeteoCommand::HardwareLogFileSegment					;
	commandMap[0x04020502] = MeteoCommand::AckHardwareLogFileSegment				;
	commandMap[0x02010503] = MeteoCommand::RequestRewriteHardwareLogFileSegment		;
	commandMap[0x02100504] = MeteoCommand::FinishWriteHardwareLog					;
	commandMap[0x02020504] = MeteoCommand::AckFinishWriteHardwareLog				;
	commandMap[0x02000505] = MeteoCommand::DeleteHardwareLog						;
	commandMap[0x02120505] = MeteoCommand::AckDeleteHardwareLog						;
	commandMap[0x02000506] = MeteoCommand::SaveHardwareLog							;
	commandMap[0x02120506] = MeteoCommand::AckSaveHardwareLog						;
	commandMap[0x02000507] = MeteoCommand::AppSwitchInstantLog						;
	commandMap[0x02120507] = MeteoCommand::AckAppSwitchInstantLog					;
	commandMap[0x02110508] = MeteoCommand::InstantLog								;
	commandMap[0x02100509] = MeteoCommand::HardwareStopInstantLog					;
	commandMap[0x02020509] = MeteoCommand::AckHardwareStopInstantLog				;
	commandMap[0x0211050A] = MeteoCommand::HardwareErrorLog							;
	// 設定	  
	commandMap[0x02000600] = MeteoCommand::ReadHardwareConfiguration	;		
	commandMap[0x02140600] = MeteoCommand::ReturnHardwareConfiguration	;		
	commandMap[0x02000601] = MeteoCommand::WriteHardwareConfiguration	;	
	commandMap[0x02120601] = MeteoCommand::AckWriteHardwareConfiguration;	
	commandMap[0x02000602] = MeteoCommand::ReadHardwareRecord			;	
	commandMap[0x02140602] = MeteoCommand::ReturnHardwareRecord			;
	// 場警
	commandMap[0x02000700] = MeteoCommand::ReadScene				;					
	commandMap[0x02140700] = MeteoCommand::ReturnScene				;	
	commandMap[0x02000701] = MeteoCommand::ReadSceneAvailability	;
	commandMap[0x02140701] = MeteoCommand::ReturnSceneAvailability	;	
	commandMap[0x02100702] = MeteoCommand::TempScene				;
	commandMap[0x02120702] = MeteoCommand::AckTempScene				;
	commandMap[0x02000703] = MeteoCommand::EnterScene				;
	commandMap[0x02120703] = MeteoCommand::AckEnterScene			;
	
	// 音樂遊戲
	// 遊戲準備
	
	commandMap[0x02000800] = MeteoCommand::ReadGameConfiguration				;					
	commandMap[0x02140800] = MeteoCommand::ReturnGameConfiguration				;	
	commandMap[0x02000801] = MeteoCommand::WriteGameConfiguration				;
	commandMap[0x02120801] = MeteoCommand::AckWriteGameConfiguration			;
	commandMap[0x02000802] = MeteoCommand::ClearGameConfiguration				;
	commandMap[0x02120802] = MeteoCommand::AckClearGameConfiguration			;
	commandMap[0x02000803] = MeteoCommand::SheetmusicData						;
	commandMap[0x02120803] = MeteoCommand::AckSheetmusicData					;
	commandMap[0x02100804] = MeteoCommand::RequestSheetmusicFile				;
	commandMap[0x02020804] = MeteoCommand::AckRequestSheetmusicFile				;
	commandMap[0x03000805] = MeteoCommand::SheetmusicFileSegment				;
	commandMap[0x04120805] = MeteoCommand::AckSheetmusicFileSegment				;
	commandMap[0x02110806] = MeteoCommand::RequestRewriteSheetmusicFileSegment	;	
	commandMap[0x02000808] = MeteoCommand::FinishWriteSheetmusic				;
	commandMap[0x02120808] = MeteoCommand::AckFinishWriteSheetmusic				;
	commandMap[0x02000809] = MeteoCommand::RequestLoadGame						;	
	commandMap[0x02120809] = MeteoCommand::AckRequestLoadGame					;
	// 遊戲進行
	commandMap[0x02110810] = MeteoCommand::HardwareGameEvent		;		
	commandMap[0x02010810] = MeteoCommand::AppGameEvent				;
	commandMap[0x02100811] = MeteoCommand::StartGame				;
	commandMap[0x02020811] = MeteoCommand::AckStartGame				;
	commandMap[0x02110812] = MeteoCommand::GameTime					;
	commandMap[0x02000813] = MeteoCommand::AppChangeGameTime		;
	commandMap[0x02120813] = MeteoCommand::AckAppChangeGameTime		;
	commandMap[0x02000814] = MeteoCommand::AppPauseGame				;
	commandMap[0x02120814] = MeteoCommand::AckAppPauseGame			;
	commandMap[0x02000815] = MeteoCommand::AppResumeGame			;
	commandMap[0x02120815] = MeteoCommand::AckAppResumeGame			;
	commandMap[0x02100816] = MeteoCommand::HardwareTerminateGame	;
	commandMap[0x02020816] = MeteoCommand::AckHardwareTerminateGame	;
	commandMap[0x02000817] = MeteoCommand::AppQuitGame				;
	commandMap[0x02120817] = MeteoCommand::AckAppQuitGame			;
	commandMap[0x02000818] = MeteoCommand::AppRestartGame			;
	commandMap[0x02120818] = MeteoCommand::AckAppRestartGame		;
	commandMap[0x02000819] = MeteoCommand::AppCompleteGame			;
	commandMap[0x02120819] = MeteoCommand::AckAppCompleteGame		;

	// 遊戲結束
	commandMap[0x02100830] = MeteoCommand::HardwareCompleteGame					;	
	commandMap[0x02020830] = MeteoCommand::AckHardwareCompleteGame				;
	commandMap[0x02100831] = MeteoCommand::FinalScore							;
	commandMap[0x02020831] = MeteoCommand::AckFinalScore						;
	commandMap[0x02100832] = MeteoCommand::PlayRecordData						;
	commandMap[0x02020832] = MeteoCommand::AckPlayRecordData					;
	commandMap[0x02000833] = MeteoCommand::RequestPlayRecordFile				;
	commandMap[0x02120833] = MeteoCommand::AckRequestPlayRecordFile				;
	commandMap[0x03100834] = MeteoCommand::PlayRecordFileSegment				;
	commandMap[0x04020834] = MeteoCommand::AckPlayRecordFileSegment				;
	commandMap[0x02010835] = MeteoCommand::RequestRewritePlayRecordFileSegment	;	
	commandMap[0x02100836] = MeteoCommand::FinishWritePlayRecord				;
	commandMap[0x02020836] = MeteoCommand::AckFinishWritePlayRecord				;

	// 識別
	SetReadFirmwareVersionPacketTypeCommand(MeteoCommand::ReadFirmwareVersion);
	SetReturnFirmwareVersionPacketTypeCommand(MeteoCommand::ReturnFirmwareVersion);
	SetJsonPacketTypeCommand(MeteoCommand::ReadFirmwareData					);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnFirmwareData				);
	SetJsonPacketTypeCommand(MeteoCommand::ReadHardwareData					);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnHardwareData				);
	SetJsonPacketTypeCommand(MeteoCommand::SendEncryptedIdentifyMessage		);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnDecryptedIdentifyMessage	);
	SetJsonPacketTypeCommand(MeteoCommand::FaultIdentity					);
	SetJsonPacketTypeCommand(MeteoCommand::TestMtu							);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnTestMtu					);
	SetJsonPacketTypeCommand(MeteoCommand::ReadAppData						);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnAppData					);
	SetJsonPacketTypeCommand(MeteoCommand::ReadUserData						);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnUserData					);
	SetJsonPacketTypeCommand(MeteoCommand::ChangeUserData					);
	SetJsonPacketTypeCommand(MeteoCommand::AckChangeUserData				);
	// 系統
	SetJsonPacketTypeCommand(MeteoCommand::RestartMainboard			);			
	SetJsonPacketTypeCommand(MeteoCommand::AckRestartMainboard		);	
	SetJsonPacketTypeCommand(MeteoCommand::RestartControlboard		);	
	SetJsonPacketTypeCommand(MeteoCommand::AckRestartControlboard	);	
	SetJsonPacketTypeCommand(MeteoCommand::RestartApp				);	
	SetJsonPacketTypeCommand(MeteoCommand::AckRestartApp			);	
	SetJsonPacketTypeCommand(MeteoCommand::RestartKeyboardProgram	);	
	SetJsonPacketTypeCommand(MeteoCommand::AckRestartKeyboardProgram);	
	SetJsonPacketTypeCommand(MeteoCommand::KeyboardErrorMessage		);
	// 心跳
	SetJsonPacketTypeCommand(MeteoCommand::SetPingPongFrequency		);
	SetJsonPacketTypeCommand(MeteoCommand::AckSetPingPongFrequency	);
	SetJsonPacketTypeCommand(MeteoCommand::Ping						);
	SetJsonPacketTypeCommand(MeteoCommand::Pong						);
	SetJsonPacketTypeCommand(MeteoCommand::ReadAppTime				);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnAppTime			);
	// 基本功能
	SetJsonPacketTypeCommand(MeteoCommand::AppIOEvent						);				
	SetJsonPacketTypeCommand(MeteoCommand::KeyboardIOEvent					);	 
	SetJsonPacketTypeCommand(MeteoCommand::AppReadKeyboardInstrument		);	 
	SetJsonPacketTypeCommand(MeteoCommand::ReturnAppReadKeyboardInstrument	);	
	SetJsonPacketTypeCommand(MeteoCommand::AppSwitchKeyboardInstrument		);	 
	SetJsonPacketTypeCommand(MeteoCommand::AckAppSwitchKeyboardInstrument	);	
	SetJsonPacketTypeCommand(MeteoCommand::KeyboardFinishSwitchInstrument	);	
	SetJsonPacketTypeCommand(MeteoCommand::AckKeyboardFinishSwitchInstrument);	 

	// 擴充升級
	SetJsonPacketTypeCommand(MeteoCommand::NewFirmwareData							);						
	SetJsonPacketTypeCommand(MeteoCommand::AckNewFirmwareData						);		
	SetJsonPacketTypeCommand(MeteoCommand::ClearNewFirmwareSplit					);
	SetJsonPacketTypeCommand(MeteoCommand::AckClearNewFirmwareSplit					);
	SetJsonPacketTypeCommand(MeteoCommand::RequestNewFirmwareSplit					);		
	SetJsonPacketTypeCommand(MeteoCommand::AckRequestNewFirmwareSplit				);	
	SetFilePacketTypeCommand(MeteoCommand::NewFirmwareSplitFileSegment				);
	SetAckFilePacketTypeCommand(MeteoCommand::AckNewFirmwareSplitFileSegment		);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteNewFirmwareSplitFileSegment);		
	SetJsonPacketTypeCommand(MeteoCommand::FinishWriteNewFirmwareSplit				);	
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWriteNewFirmwareSplit			);	
	SetJsonPacketTypeCommand(MeteoCommand::FinishRequestNewFirmware					);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishRequestNewFirmware				);	
	

	// log
	SetJsonPacketTypeCommand(MeteoCommand::ChangeHardwareLogLevel				);		
	SetJsonPacketTypeCommand(MeteoCommand::AckChangeHardwareLogLevel			);
	SetJsonPacketTypeCommand(MeteoCommand::DownloadHardwareLog					);
	SetJsonPacketTypeCommand(MeteoCommand::AckDownloadHardwareLog				);
	SetFilePacketTypeCommand(MeteoCommand::HardwareLogFileSegment				);
	SetAckFilePacketTypeCommand(MeteoCommand::AckHardwareLogFileSegment			);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteHardwareLogFileSegment	);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWriteHardwareLog				);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWriteHardwareLog			);
	SetJsonPacketTypeCommand(MeteoCommand::DeleteHardwareLog					);
	SetJsonPacketTypeCommand(MeteoCommand::AckDeleteHardwareLog					);
	SetJsonPacketTypeCommand(MeteoCommand::SaveHardwareLog						);
	SetJsonPacketTypeCommand(MeteoCommand::AckSaveHardwareLog					);
	SetJsonPacketTypeCommand(MeteoCommand::AppSwitchInstantLog					);
	SetJsonPacketTypeCommand(MeteoCommand::AckAppSwitchInstantLog				);
	SetJsonPacketTypeCommand(MeteoCommand::InstantLog							);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareStopInstantLog				);
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwareStopInstantLog			);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareErrorLog						);
	
	
	// 設定
	SetJsonPacketTypeCommand(MeteoCommand::ReadHardwareConfiguration		);	
	SetJsonPacketTypeCommand(MeteoCommand::ReturnHardwareConfiguration		);	
	SetJsonPacketTypeCommand(MeteoCommand::WriteHardwareConfiguration		);	
	SetJsonPacketTypeCommand(MeteoCommand::AckWriteHardwareConfiguration	);	
	SetJsonPacketTypeCommand(MeteoCommand::ReadHardwareRecord				);	
	SetJsonPacketTypeCommand(MeteoCommand::ReturnHardwareRecord				);
	// 場警
	SetJsonPacketTypeCommand(MeteoCommand::ReadScene				);				
	SetJsonPacketTypeCommand(MeteoCommand::ReturnScene				);
	SetJsonPacketTypeCommand(MeteoCommand::ReadSceneAvailability	);
	SetJsonPacketTypeCommand(MeteoCommand::ReturnSceneAvailability	);
	SetJsonPacketTypeCommand(MeteoCommand::TempScene				);
	SetJsonPacketTypeCommand(MeteoCommand::AckTempScene				);
	SetJsonPacketTypeCommand(MeteoCommand::EnterScene				);
	SetJsonPacketTypeCommand(MeteoCommand::AckEnterScene			);
	// 音樂遊戲
	// 遊戲準備
	SetJsonPacketTypeCommand(MeteoCommand::ReadGameConfiguration				);				
	SetJsonPacketTypeCommand(MeteoCommand::ReturnGameConfiguration				);
	SetJsonPacketTypeCommand(MeteoCommand::WriteGameConfiguration				);
	SetJsonPacketTypeCommand(MeteoCommand::AckWriteGameConfiguration			);
	SetJsonPacketTypeCommand(MeteoCommand::ClearGameConfiguration				);
	SetJsonPacketTypeCommand(MeteoCommand::AckClearGameConfiguration			);
	SetJsonPacketTypeCommand(MeteoCommand::SheetmusicData						);
	SetJsonPacketTypeCommand(MeteoCommand::AckSheetmusicData					);
	SetJsonPacketTypeCommand(MeteoCommand::RequestSheetmusicFile				);
	SetJsonPacketTypeCommand(MeteoCommand::AckRequestSheetmusicFile				);
	SetFilePacketTypeCommand(MeteoCommand::SheetmusicFileSegment				);
	SetAckFilePacketTypeCommand(MeteoCommand::AckSheetmusicFileSegment			);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewriteSheetmusicFileSegment	);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWriteSheetmusic				);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWriteSheetmusic				);
	SetJsonPacketTypeCommand(MeteoCommand::RequestLoadGame						);
	SetJsonPacketTypeCommand(MeteoCommand::AckRequestLoadGame					);
	// 遊戲進行
	SetJsonPacketTypeCommand(MeteoCommand::HardwareGameEvent		);					
	SetJsonPacketTypeCommand(MeteoCommand::AppGameEvent				);
	SetJsonPacketTypeCommand(MeteoCommand::StartGame				);		
	SetJsonPacketTypeCommand(MeteoCommand::AckStartGame				);
	SetJsonPacketTypeCommand(MeteoCommand::GameTime					);
	SetJsonPacketTypeCommand(MeteoCommand::AppChangeGameTime		);		
	SetJsonPacketTypeCommand(MeteoCommand::AckAppChangeGameTime		);
	SetJsonPacketTypeCommand(MeteoCommand::AppPauseGame				);
	SetJsonPacketTypeCommand(MeteoCommand::AckAppPauseGame			);		
	SetJsonPacketTypeCommand(MeteoCommand::AppResumeGame			);		
	SetJsonPacketTypeCommand(MeteoCommand::AckAppResumeGame			);
	SetJsonPacketTypeCommand(MeteoCommand::HardwareTerminateGame	);		
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwareTerminateGame	);
	SetJsonPacketTypeCommand(MeteoCommand::AppQuitGame				);		
	SetJsonPacketTypeCommand(MeteoCommand::AckAppQuitGame			);		
	SetJsonPacketTypeCommand(MeteoCommand::AppRestartGame			);		
	SetJsonPacketTypeCommand(MeteoCommand::AckAppRestartGame		);		
	SetJsonPacketTypeCommand(MeteoCommand::AppCompleteGame			);		
	SetJsonPacketTypeCommand(MeteoCommand::AckAppCompleteGame		);		
	// 遊戲結束
	SetJsonPacketTypeCommand(MeteoCommand::HardwareCompleteGame					);	
	SetJsonPacketTypeCommand(MeteoCommand::AckHardwareCompleteGame				);
	SetJsonPacketTypeCommand(MeteoCommand::FinalScore							);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinalScore						);
	SetJsonPacketTypeCommand(MeteoCommand::PlayRecordData						);
	SetJsonPacketTypeCommand(MeteoCommand::AckPlayRecordData					);
	SetJsonPacketTypeCommand(MeteoCommand::RequestPlayRecordFile				);
	SetJsonPacketTypeCommand(MeteoCommand::AckRequestPlayRecordFile				);
	SetFilePacketTypeCommand(MeteoCommand::PlayRecordFileSegment				);
	SetAckFilePacketTypeCommand(MeteoCommand::AckPlayRecordFileSegment			);
	SetJsonPacketTypeCommand(MeteoCommand::RequestRewritePlayRecordFileSegment	);
	SetJsonPacketTypeCommand(MeteoCommand::FinishWritePlayRecord				);
	SetJsonPacketTypeCommand(MeteoCommand::AckFinishWritePlayRecord				);

}

int MeteoPacketConverterV2::SplitPacket(const char * bufferIn, int bytesRead, char ** packets, int * packerLengths)
{
	// depricated
	return 0;
}

PacketStatus MeteoPacketConverterV2::CheckPacketStatus(const char * packet, int length)
{

	LOG(LogLevel::Debug) << "MeteoPacketConverterV1::CheckPacketStatus() : packet [" << length << "].";

	unsigned long command;
	memcpy(&command, packet, sizeof(command));
	unsigned short packetLength = 0;
	memcpy(&packetLength, packet + sizeof(command), sizeof(packetLength));


	LOG(LogLevel::Debug) << "MeteoPacketConverterV1::CheckPacketStatus() : command [" << hex << command << dec << "], length[" << packetLength << "].";

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

	map<unsigned int, MeteoCommand>::iterator iter;
	iter = commandMap.find(command);
	if (iter == commandMap.end()) {
		return PacketStatus::WrongCommand;
	}

	// TODO: wrong json format

	// TODO: damaged

	return PacketStatus::Fine;
}

PacketType MeteoPacketConverterV2::CheckPacketType(const char * buffer, int size)
{
	unsigned long command = 0x0;
	memcpy(&command, buffer, sizeof(command));

	map<unsigned int, MeteoCommand>::iterator iter;
	iter = commandMap.find(command);
	if (iter != commandMap.end()) {

		map<MeteoCommand, PacketType>::iterator iter2;
		iter2 = CommandPacketTypeMap.find(commandMap[command]);
		if(iter2 != CommandPacketTypeMap.end())
			return CommandPacketTypeMap[commandMap[command]];
	}

	return PacketType::None;
}

PacketType MeteoPacketConverterV2::CheckCommandType(BluetoothMessage * bluetoothCommand)
{
	MeteoCommand command = dynamic_cast<MeteoBluetoothMessage*>(bluetoothCommand)->GetCommand();
	
	map<MeteoCommand, PacketType>::iterator iter;
	iter = CommandPacketTypeMap.find(command);
	if (iter != CommandPacketTypeMap.end())
		return CommandPacketTypeMap[command];

	return PacketType::None;
}

BluetoothMessage * MeteoPacketConverterV2::ConvertToBluetoothMessage(const char * buffer, int size)
{
	unsigned int command = 0x0;
	memcpy(&command, buffer, sizeof(command));

	LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::ConvertToBluetoothMessage() : command [" << hex << command << dec << "], length[" << size << "].";
	LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::ConvertToBluetoothMessage() : packet [" << string(buffer, size) << "].";

	map<unsigned int, MeteoCommand>::iterator iter;
	iter = commandMap.find(command);
	if (iter != commandMap.end()) {

		if (CheckPacketType(buffer, size) == PacketType::Json) {

			MeteoContextBluetoothMessage* btMessage = new MeteoContextBluetoothMessage(commandMap[command]);

			LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::ConvertToBluetoothMessage() : command [" << (int)buffer[0] << " " << (int)buffer[1] << " " << (int)buffer[2] << " " << (int)buffer[3] << "].";
			LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::ConvertToBluetoothMessage() : id [" << (int)buffer[4] << " " << (int)buffer[5] << "].";
			LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::ConvertToBluetoothMessage() : length [" << (int)buffer[6] << " " << (int)buffer[7] << "].";
			LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::ConvertToBluetoothMessage() : order [" << (int)buffer[8] << " " << (int)buffer[9] << "].";
			LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::ConvertToBluetoothMessage() : amount [" << (int)buffer[10] << " " << (int)buffer[11] << "].";

			unsigned short length;
			memcpy(&length, buffer + sizeof(command) + sizeof(unsigned short), sizeof(length));
			LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::ConvertToBluetoothMessage() : length [" << length << "].";
			
			memset(contextBuffer, 0, sizeof(char) * 1024);
			memcpy(contextBuffer,
				buffer + sizeof(command) + sizeof(length) + sizeof(unsigned short) * 3,
				length - (sizeof(command) + sizeof(length) + sizeof(unsigned short) * 3));
			// Id:unisgned int(4)
			// Length:unsigned short 長度(2)
			// Order : unsigned short 順序(2)
			// Amount : unsigned short 封包數(2)
			// Text : char[] 內文

			LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::ConvertToBluetoothMessage() : context [" << string(contextBuffer, length - (sizeof(command) + sizeof(length) + sizeof(unsigned short) * 3)) << "].";

			try {
				json context = json::parse(contextBuffer);

				btMessage->SetContextInJson(context);
				btMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
				LOG(LogLevel::Depricated) << "MeteoPacketConverterV2::ConvertToBluetoothMessage() : json context [" << context.dump() << "].";
			}
			catch (exception& e) {
				if (string(contextBuffer) == "{}") {
					LOG(LogLevel::Depricated) << "MeteoPacketConverterV2::ConvertToBluetoothMessage() : empty json.";

					json context;

					btMessage->SetContextInJson(context);
					btMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

					return btMessage;
				}

				LOG(LogLevel::Error) << "MeteoPacketConverterV2::ConvertToBluetoothMessage() : parse json error : " << e.what();

				delete btMessage;
				return nullptr;
			}

			LOG(LogLevel::Depricated) << "MeteoPacketConverterV2::ConvertToBluetoothMessage() : json context [" << btMessage->GetContext() << "].";

			//LOG(LogLevel::Debug) << "MeteoPacketConverterV1::ConvertToBluetoothCommand() : command [" << hex << command << dec << "], context [" << btCommand->GetContext().dump() << "].";

			// TODO: parse失敗要error handle

			return btMessage;
		}
	}

	return nullptr;
}

int MeteoPacketConverterV2::GetCountOfPacket(BluetoothMessage * bluetoothCommand)
{
	if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(bluetoothCommand)) {
		return dynamic_cast<MeteoFileSegmentBluetoothMessage*>(bluetoothCommand)->GetAmount();
	}

	// 目前一班封包通常只有不會切成兩個
	return 1;
}

int MeteoPacketConverterV2::ConvertToByteArray(BluetoothMessage * bluetoothCommand, int packetOrder, char * buffer, int bufferMaxSize)
{
	// 改成寫在bluetooth phone裡面了


	// TODO: 丟出去的部分還沒寫
	return 0;
}

int MeteoPacketConverterV2::ConvertToByteArray(BluetoothMessage * bluetoothMessage, char * buffer, int bufferMaxSize)
{
	LOG(LogLevel::Depricated) << "MeteoPacketConverterV2::ConvertToByteArray() : convert new message.";
	if (dynamic_cast<MeteoContextBluetoothMessage*>(bluetoothMessage)) {
		MeteoContextBluetoothMessage* contextBluetoothMessage = dynamic_cast<MeteoContextBluetoothMessage*>(bluetoothMessage);

		LOG(LogLevel::Depricated) << "MeteoPacketConverterV2::ConvertToByteArray() : convert context message.";

		string context = contextBluetoothMessage->GetContext();

		LOG(LogLevel::Depricated) << "MeteoPacketConverterV2::ConvertToByteArray() : context [" << context << "].";

		unsigned short bufferSize = context.length() + 12;
		if (bufferSize > bufferMaxSize) {
			LOG(LogLevel::Warning) << "MeteoPacketConverterV2::ConvertToByteArray() : message oversize [" << context << "].";
			return -1;
		}

#ifdef DEBUG_VARIANT
		if (contextBluetoothMessage->GetCommand() == MeteoCommand::HardwareGameEvent) {
			return -1;
			LOG(LogLevel::Debug) << "MeteoPacketConverterV2::ConvertToByteArray() : context [" << context << "].";
		}
#endif

		memset(buffer, 0, bufferMaxSize);

		unsigned int command;

		map<unsigned int, MeteoCommand>::iterator iter;
		for (iter = commandMap.begin(); iter != commandMap.end(); ++iter) {
			if (iter->second == contextBluetoothMessage->GetCommand()) {
				command = iter->first;
			}
		}
		memcpy(buffer, &command, sizeof(command));
		LOG(LogLevel::Depricated) << "MeteoPacketConverterV2::ConvertToByteArray() : command [" << command << "] to byte is [" << (int)buffer[0] << " " << (int)buffer[1] << " " << (int)buffer[2] << " " << (int)buffer[3] << "].";


		memcpy(buffer + sizeof(command)								, &tempPacketId	, sizeof(tempPacketId));

		memcpy(buffer + sizeof(command) + sizeof(unsigned short)	, &bufferSize	, sizeof(bufferSize));

		unsigned short order = 0;
		memcpy(buffer + sizeof(command) + sizeof(unsigned short) * 2, &order		, sizeof(order));

		unsigned short amount = 1;
		memcpy(buffer + sizeof(command) + sizeof(unsigned short) * 3, &amount		, sizeof(amount));

		const char *contextInCharArray = context.c_str();
		memcpy(buffer + sizeof(command) + sizeof(unsigned short) * 4, contextInCharArray, sizeof(contextInCharArray) * context.length());
		
		tempPacketId++;

#ifdef DEBUG_VARIANT
		if (contextBluetoothMessage->GetCommand() == MeteoCommand::HardwareGameEvent) {
			LOG(LogLevel::Debug) << "MeteoPacketConverterV2::ConvertToByteArray() : context [" << buffer << "] converted.";
		}
#endif

		return bufferSize;
	}
	else if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(bluetoothMessage)) {

		MeteoFileSegmentBluetoothMessage* fileBluetoothMessage = dynamic_cast<MeteoFileSegmentBluetoothMessage*>(bluetoothMessage);

		unsigned short bufferSize = fileBluetoothMessage->GetFileSegmentSize() + 28;

		if(bufferSize > bufferMaxSize) {
			LOG(LogLevel::Warning) << "MeteoPacketConverterV1::ConvertToByteArray() : message oversize [" << fileBluetoothMessage->GetFileName() << "] order [" << fileBluetoothMessage->GetFileSegmentSize() << "].";
			return -1;
		}

		memset(buffer, 0, bufferMaxSize);

		unsigned int command;

		map<unsigned int, MeteoCommand>::iterator iter;
		for (iter = commandMap.begin(); iter != commandMap.end(); ++iter) {
			if (iter->second == fileBluetoothMessage->GetCommand()) {
				command = iter->first;
			}
		}
		memcpy(buffer, &command, sizeof(command));

		memcpy(buffer + sizeof(command)										, &tempPacketId, sizeof(tempPacketId));

		memcpy(buffer + sizeof(command) + sizeof(unsigned short)			, &bufferSize, sizeof(bufferSize));

		const char *fileNameInCharArray = fileBluetoothMessage->GetFileName().c_str();
		memcpy(buffer + sizeof(command) + sizeof(unsigned short) * 2		, fileNameInCharArray, strlen(fileNameInCharArray) > 16 ? 16 : strlen(fileNameInCharArray));

		unsigned short order = fileBluetoothMessage->GetOrder();
		memcpy(buffer + sizeof(command) + sizeof(unsigned short) * 2 + 16	, &order, sizeof(order));

		unsigned short amount = fileBluetoothMessage->GetAmount();
		memcpy(buffer + sizeof(command) + sizeof(unsigned short) * 3 + 16	, &amount, sizeof(amount));

		memcpy(buffer + sizeof(command) + sizeof(unsigned short) * 4 + 16	, fileBluetoothMessage->GetFileSegment(), fileBluetoothMessage->GetFileSegmentSize());

		tempPacketId++;
		return bufferSize;
	}
	else if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(bluetoothMessage)) {

		MeteoAckFileSegmentBluetoothMessage* ackFileBluetoothMessage = dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(bluetoothMessage);


		memset(buffer, 0, bufferMaxSize);
		unsigned int command;

		map<unsigned int, MeteoCommand>::iterator iter;
		for (iter = commandMap.begin(); iter != commandMap.end(); ++iter) {
			if (iter->second == ackFileBluetoothMessage->GetCommand()) {
				command = iter->first;
			}
		}
		memcpy(buffer, &command, sizeof(command));

		memcpy(buffer + sizeof(command), &tempPacketId, sizeof(tempPacketId));

		unsigned short size = 28;
		memcpy(buffer + sizeof(command) + sizeof(unsigned short), &size, sizeof(unsigned short));

		const char *fileNameInCharArray = ackFileBluetoothMessage->GetFileName().c_str();
		memcpy(buffer + sizeof(command) + sizeof(unsigned short) * 2, fileNameInCharArray, ackFileBluetoothMessage->GetFileName().length() > 16 ? 16 : ackFileBluetoothMessage->GetFileName().length());

		unsigned short order = ackFileBluetoothMessage->GetOrder();
		memcpy(buffer + sizeof(command) + sizeof(unsigned short) * 2 + 16, &order, sizeof(order));

		unsigned short amount = ackFileBluetoothMessage->GetAmount();
		memcpy(buffer + sizeof(command) + sizeof(unsigned short) * 3 + 16, &amount, sizeof(amount));

		tempPacketId++;
		return size;

	}

	LOG(LogLevel::Warning) << "MeteoPacketConverterV2::ConvertToByteArray() : message not convertable.";
	return -1;
}

BluetoothMessage* MeteoPacketConverterV2::ConvertToFile(const char * buffer, int size)
{
	unsigned int command = 0x0;
	memcpy(&command, buffer, sizeof(command));

	map<unsigned int, MeteoCommand>::iterator iter;
	iter = commandMap.find(command);
	if (iter != commandMap.end()) {

		LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::ConvertToFile() : converting [" << command << "] command file.";

		if (CheckPacketType(buffer, size) == PacketType::File) {

			//int id =  getId(buffer, size);

			string fileName = getFileName(buffer, size);
			int fileSegmentSize = getFileSize(buffer, size);

			if (fileSegmentSize < 0 || fileSegmentSize > maxFileSegmentSize) {
				LOG(LogLevel::Error) << "MeteoPacketConverterV2::ConvertToFile() : wrong file size [" << fileSegmentSize << "].";
				return nullptr;
			}


			char* fileSegment = getFileSegment(buffer, size);
			int fileSegmentNumber = getFileSegmentOrder(buffer, size);
			int fileSegmentCount = getFileSegmentCount(buffer, size);

			MeteoFileSegmentBluetoothMessage* fileSegmentBluetoothMessage = new MeteoFileSegmentBluetoothMessage(commandMap[command], fileSegment, fileSegmentSize, fileName, fileSegmentNumber, fileSegmentCount);

			delete[] fileSegment;

			LOG(LogLevel::Depricated) << "MeteoPacketConverterV2::ConvertToFile() : get file [" << fileName << "] segment [" << fileSegmentNumber << "]/[" << fileSegmentCount << "], size [" << fileSegmentSize << "].";

			return fileSegmentBluetoothMessage;
		}
	}

	LOG(LogLevel::Error) << "MeteoPacketConverterV2::ConvertToFile() : convert failed .";
	return nullptr;
}

BluetoothMessage * MeteoPacketConverterV2::ConvertToAckFileMessage(const char * buffer, int size)
{
	unsigned int command = 0x0;
	memcpy(&command, buffer, sizeof(command));

	map<unsigned int, MeteoCommand>::iterator iter;
	iter = commandMap.find(command);
	if (iter != commandMap.end()) {

		LOG(LogLevel::Fine) << "MeteoPacketConverterV1::ConvertToAckFileMessage() : converting [" << command << "] command file.";

		if (CheckPacketType(buffer, size) == PacketType::AckFile) {

			string fileName = getFileName(buffer, size);

			int fileSegmentNumber = getFileSegmentOrder(buffer, size);
			int fileSegmentCount = getFileSegmentCount(buffer, size);

			MeteoAckFileSegmentBluetoothMessage* fileSegmentBluetoothMessage = new MeteoAckFileSegmentBluetoothMessage(commandMap[command], fileName, fileSegmentNumber, fileSegmentCount);


			LOG(LogLevel::Depricated) << "MeteoPacketConverterV2::ConvertToAckFileMessage() : get file [" << fileName << "] segment [" << fileSegmentNumber << "]/[" << fileSegmentCount << "].";

			return fileSegmentBluetoothMessage;
		}
	}

	LOG(LogLevel::Error) << "MeteoPacketConverterV2::ConvertToAckFileMessage() : convert failed .";
	return nullptr;
}
