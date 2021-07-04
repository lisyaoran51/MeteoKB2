#ifndef METEO_COMMAND_H
#define METEO_COMMAND_H



namespace Games {
namespace Input{
namespace Commands{


	enum class MeteoCommand {
		None = -1				,
		// �ѧO
		ReadFirmwareVersion		,
		ReturnFirmwareVersion	,
		ReadFirmwareData		,
		ReturnFirmwareData		,
		ReadHardwareData		,
		ReturnHardwareData		,
		SendEncryptedIdentifyMessage,
		ReturnDecryptedIdentifyMessage,
		FaultIdentity			,
		TestMtu					,
		ReturnTestMtu			,
		ReadAppData				,
		ReturnAppData			,
		ReadUserData			,
		ReturnUserData			,
		ChangeUserData			,
		AckChangeUserData		,
		// �t��
		RestartMainboard = 100		,
		AckRestartMainboard			,
		RestartControlboard			,
		AckRestartControlboard		,
		RestartApp					,
		AckRestartApp				,
		RestartKeyboardProgram		,
		AckRestartKeyboardProgram	,
		KeyboardErrorMessage		,
		// �߸�
		SetPingPongFrequency = 200	,
		AckSetPingPongFrequency		,
		Ping						,
		Pong						,
		ReadAppTime					,
		ReturnAppTime				,
		// �򥻥\��
		AppIOEvent = 300					,
		KeyboardIOEvent						,
		AppReadKeyboardInstrument			,
		ReturnAppReadKeyboardInstrument		,
		AppSwitchKeyboardInstrument			,
		AckAppSwitchKeyboardInstrument		,
		KeyboardFinishSwitchInstrument		,
		AckKeyboardFinishSwitchInstrument	,
		// �X�R�ɯ�
		NewFirmwareData = 400						,
		AckNewFirmwareData							,
		ClearNewFirmwareSplit						,
		AckClearNewFirmwareSplit					,
		RequestNewFirmwareSplit						,
		AckRequestNewFirmwareSplit					,
		NewFirmwareSplitFileSegment					,
		AckNewFirmwareSplitFileSegment				,
		RequestRewriteNewFirmwareSplitFileSegment	,
		FinishWriteNewFirmwareSplit					,
		AckFinishWriteNewFirmwareSplit				,
		FinishRequestNewFirmware					,
		AckFinishRequestNewFirmware					,

		// log
		ChangeHardwareLogLevel = 500		,
		AckChangeHardwareLogLevel			,
		DownloadHardwareLog					,
		AckDownloadHardwareLog				,
		HardwareLogFileSegment				,
		AckHardwareLogFileSegment			,
		RequestRewriteHardwareLogFileSegment,
		FinishWriteHardwareLog				,
		AckFinishWriteHardwareLog			,
		DeleteHardwareLog					,
		AckDeleteHardwareLog				,
		SaveHardwareLog						,
		AckSaveHardwareLog					,
		AppSwitchInstantLog					,
		AckAppSwitchInstantLog				,
		InstantLog							,
		HardwareStopInstantLog				,
		AckHardwareStopInstantLog			,
		HardwareErrorLog					,


		// �]�w
		ReadHardwareConfiguration = 600	,
		ReturnHardwareConfiguration		,
		WriteHardwareConfiguration		,
		AckWriteHardwareConfiguration	,
		ReadHardwareRecord				,
		ReturnHardwareRecord			,
		// ��ĵ
		ReadScene = 700			,
		ReturnScene				,
		ReadSceneAvailability	,
		ReturnSceneAvailability	,
		TempScene				,
		AckTempScene			,
		EnterScene				,
		AckEnterScene			,

		// ���ֹC��
		// �C���ǳ�
		ReadGameConfiguration = 800			,
		ReturnGameConfiguration				,
		WriteGameConfiguration				,
		AckWriteGameConfiguration			,
		ClearGameConfiguration				,
		AckClearGameConfiguration			,
		SheetmusicData						,
		AckSheetmusicData					,
		RequestSheetmusicFile				,
		AckRequestSheetmusicFile			,
		SheetmusicFileSegment				,
		AckSheetmusicFileSegment			,
		RequestRewriteSheetmusicFileSegment	,
		FinishWriteSheetmusic				,
		AckFinishWriteSheetmusic			,
		RequestLoadGame						,
		AckRequestLoadGame					,

		// �C���i��
		HardwareGameEvent			,
		AppGameEvent				,
		StartGame					,
		AckStartGame				,
		GameTime					,
		AppChangeGameTime			,
		AckAppChangeGameTime		,
		AppPauseGame				,
		AckAppPauseGame				,
		AppResumeGame				,
		AckAppResumeGame			,
		HardwareTerminateGame		,
		AckHardwareTerminateGame	,
		AppQuitGame					,
		AckAppQuitGame				,
		AppRestartGame				,
		AckAppRestartGame			,
		AppCompleteGame				,
		AckAppCompleteGame			,

		// �C������
		HardwareCompleteGame				,
		AckHardwareCompleteGame				,
		FinalScore							,
		AckFinalScore						,
		PlayRecordData						,
		AckPlayRecordData					,
		RequestPlayRecordFile				,
		AckRequestPlayRecordFile			,
		PlayRecordFileSegment				,
		AckPlayRecordFileSegment			,
		RequestRewritePlayRecordFileSegment	,
		FinishWritePlayRecord				,
		AckFinishWritePlayRecord			,
		// ����

		// ���ʹC��
		InstantEvent = 1000	,

		// ����
		RecordEvent	= 1100	,









	};

}}}





#endif
