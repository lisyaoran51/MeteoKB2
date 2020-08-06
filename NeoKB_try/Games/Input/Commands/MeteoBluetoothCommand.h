#ifndef METEO_BLUETOOTH_COMMAND_H
#define METEO_BLUETOOTH_COMMAND_H


#include "../../../Framework/Input/Commands/BluetoothCommand.h"


using namespace Framework::Input::Commands;


namespace Games {
namespace Input{
namespace Commands{

	enum class MeteoCommand;

	class MeteoBluetoothCommand : public TBluetoothCommand<MeteoCommand> {

	public:

		MeteoBluetoothCommand(MeteoCommand c, json text);

		MeteoBluetoothCommand(MeteoCommand c);

	};

	enum class MeteoCommand {
		// �ѧO
		ReadFirmwareVersion = 0	,
		ReturnFirmwareVersion	,
		ReadFirmwareData		,
		ReturnFirmwareData		,
		ReadHardwareData		,
		ReturnHardwareDate		,
		ReadHardwareIdentity	,
		ReturnHardwarIdentity	,
		ReadAppData				,
		ReturnAppData			,
		ReadUserData			,
		ReturnUserData			,
		ChangeUserData			,
		AckChangeUserData		,
		// �t��
		ReadMainboardStatus	= 100,
		ReturnMainboardStatus	,
		RestartMainboard		,
		AckRestartMainboard		,
		RestartControlboard		,
		AckRestartControlboard	,
		RestartApp				,
		AckRestartApp			,
		// �߸�
		SetHearbeatFrequency = 200	,
		AckSetHearbeatFrequency		,
		Ping						,
		Pong						,
		HardwareRequestTerminate	,
		AckHardwareRequestTerminate	,
		AppRequestTerminate			,
		AckAppRequestTerminate		,
		ReadAppTime					,
		ReturnAppTime				,
		// �򥻥\��
		PressPowerButton = 300		,
		AckPressPowerButton			,
		PressSustainButton			,
		AckPressSustainButton		,
		PressSensitiveButton		,
		AckPressSensitiveButton		,
		PressLowerOctaveButton		,
		AckPressLowerOctaveButton	,
		PressRaiseOctaveButton		,
		AckPressRaiseOctaveButton	,
		AdjustInstrumentVolume		,
		AckAdjustInstrumentVolume	,
		AdjustBackgroundButton		,
		AckAdjustBackgroundButton	,
		PluginPedal					,
		AckPluginPedal				,
		PluginEarphone				,
		AckPluginEarphone			,
		// �X�R�ɯ�
		NewFirmwareData = 400					,
		AckNewFirmwareData						,
		NewFirmwareFileSegment					,
		AckNewFirmwareFileSegment				,
		RequestRewriteNewFirmwareFileSegment	,
		RewriteNewFirmwareFileSegment			,
		AckRewriteNewFirmwareFileSegment		,
		FinishWriteNewFirmwareFile				,
		AckFinishWriteNewFirmwareFile			,
		NewInstrumentPackageData				,
		AckNewInstrumentPackageData				,
		NewInstrumentFileSegment				,
		AckNewInstrumentFileSegment				,
		FinishWriteNewInstrumentFile			,
		AckFinishWriteNewInstrumentFile			,
		FinishWriteNewInstrumentFilePackage		,
		AckFinishWriteNewInstrumentFilePackage	,
		RequestRewriteNewInstrumentFile			,
		RewriteNewInstrumentFile				,
		AckRewriteNewInstrumentFile				,
		RequestRewriteNewInstrumentFileSegment	,
		RewriteNewInstrumentFileSegment			,
		AckRewriteNewInstrumentFileSegment		,
		// log
		ChangeHardwareLogLevel = 500			,
		AckChangeHardwareLogLevel				,
		DownloadHardwareLog						,
		HardwareLogFileSegment					,
		AckHardwareLogFileSegment				,
		DeleteHardwareLog						,
		AckDeleteHardwareLog					,
		SaveHardwareLog							,
		AckSaveHardwareLog						,
		SwitchInstantLog						,
		AckSwitchInstantLog						,
		InstantLog								,
		StopInstantLog							,
		AckStopInstantLog						,
		// �]�w
		ReadHardwareConfiguration = 600			,
		ReturnHardwareConfiguration				,
		WriteHardwareConfiguration				,
		ReturnWriteHardwareConfiguration		,
		ReadHardwareRecord						,
		ReturnHardwareRecord					,
		// ��ĵ
		ReadScene = 700							,
		ReturnScene								,
		ReadSceneAvailability					,
		ReturnSceneAvailability					,
		EnterScene								,
		AckEnterScene							,
		ReadSubsceneAvailability				,
		RerturnSubsceneAvailability				,
		EnterSubscene							,
		AckEnterSubscene						,
		// ���ֹC��
		// �C���ǳ�
		ReadGameConfiguration = 800				,
		ReturnGameConfiguration					,
		WriteGameConfiguration					,
		ReturnWriteGameConfiguration			,
		SheetmusicData							,
		AckSheetmusicData						,
		SheetmusicFileSegment					,
		AckSheetmusicFileSegment				,
		RequestRewriteSheetmusicFileSegment		,
		RewriteSheetmusicFileSegment			,
		AckRewriteSheetmusicFileSegment			,
		FinishWriteSheetmusic					,
		AckFinishWriteSheetmusic				,
		RequestLoadGame							,
		AckRequestLoadGame						,
		FinishLoadGame							,
		AckFinishLoadGame						,
		// �C���i��
		StartGame								,
		AckStartGame							,
		GameTime								,
		HardwareChangeGameTime					,
		AckHardwareChangeGameTime				,
		AppChangeGameTime						,
		AckAppChangeGameTime					,
		HardwareChangeGameSection				,
		AckHardwareChangeGameSection			,
		AppChangeGameSection					,
		AckAppChangeGameSection					,
		HardwarePauseGame						,
		AckHardwarePauseGame					,
		AppPauseGame							,
		AckAppPauseGame							,
		HardwareResumeGame						,
		AckHardwareResumeGame					,
		AppResumeGame							,
		AckAppResumeGame						,
		PressKey								,
		ReleaseKey								,
		NowScore								,
		HardwareSwitchRepeatPracticeState		,
		HardwareSwitchRepeatPracticeRound		,
		HardwareTerminateGame					,
		AckHardwareTerminateGame				,
		AppTerminateGame						,
		AckAppTerminateGame						,
		// �C������
		HardwareFinishGame						,
		AckHardwareFinishGame					,
		FinalScore								,
		AckFinalScore							,
		PlayRecordData							,
		AckPlayRecordData						,
		PlayRecordFileSegment					,
		AckPlayRecordFileSegment				,
		RequestRewritePlayRecordFileSegment		,
		RewritePlayRecordFileSegment			,
		AckRewritePlayRecordFileSegment			,
		FinishWritePlayRecord					,
		AckFinishWritePlayRecord				,








	};

}}}





#endif
