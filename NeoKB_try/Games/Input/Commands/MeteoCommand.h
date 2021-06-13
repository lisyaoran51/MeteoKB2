#ifndef METEO_COMMAND_H
#define METEO_COMMAND_H



namespace Games {
namespace Input{
namespace Commands{


	enum class MeteoCommand {
		None = -1				,
		// 識別
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
		// 系統
		RestartMainboard = 100	,
		AckRestartMainboard		,
		RestartControlboard		,
		AckRestartControlboard	,
		// 心跳
		SetPingPongFrequency = 200	,
		AckSetPingPongFrequency		,
		Ping						,
		Pong						,
		ReadAppTime					,
		ReturnAppTime				,
		// 基本功能
		AppIOEvent = 300						,
		KeyboardIOEvent							,
		AppReadKeyboardInstruments				,
		ReturnAppReadKeyboardInstruments		,
		AppSwitchKeyboardInstrument				,
		AckAppSwitchKeyboardInstrument			,
		KeyboardFinishSwitchInstrument			,
		AckKeyboardFinishSwitchInstrument		,
		// 擴充升級
		NewFirmwareData = 400						,
		AckNewFirmwareData							,
		RequestNewFirmwareSplit,
		AckRequestNewFirmwareSplit,
		NewFirmwareSplitFileSegment					,
		AckNewFirmwareSplitFileSegment				,
		RequestRewriteNewFirmwareSplitFileSegment	,
		FinishWriteNewFirmwareSplit					,
		AckFinishWriteNewFirmwareSplit				,
		RequestRewriteNewFirmwareSplit				,
		FinishWriteNewFirmware						,
		AckFinishWriteNewFirmware					,
		// log
		ChangeHardwareLogLevel = 500		,
		AckChangeHardwareLogLevel			,
		DownloadHardwareLog					,
		AckDownloadHardwareLog				,
		HardwareLogFileSegment				,
		AckHardwareLogFileSegment			,
		RequestRewiteHardwareLogFileSegment	,
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
		// 設定
		ReadHardwareConfiguration = 600	,
		ReturnHardwareConfiguration		,
		WriteHardwareConfiguration		,
		AckWriteHardwareConfiguration	,
		ReadHardwareRecord				,
		ReturnHardwareRecord			,
		// 場警
		ReadScene = 700			,
		ReturnScene				,
		ReadSceneAvailability	,
		ReturnSceneAvailability	,
		TempScene				,
		AckTempScene			,
		EnterScene				,
		AckEnterScene			,
		// 音樂遊戲
		// 遊戲準備
		ReadGameConfiguration = 800			,
		ReturnGameConfiguration				,
		WriteGameConfiguration				,
		AckWriteGameConfiguration			,
		ClearGameConfiguration				,
		AckClearGameConfiguration			,
		SheetmusicData						,
		AckSheetmusicData					,
		RequestSheetmusicFile				,
		AckRequestSheetmusicFile			,	// 後來佳的
		SheetmusicFileSegment				,	// 後來佳的
		AckSheetmusicFileSegment			,
		RequestRewriteSheetmusicFileSegment	,
		FinishWriteSheetmusic				,
		AckFinishWriteSheetmusic			,
		RequestLoadGame						,
		AckRequestLoadGame					,
		// 遊戲進行
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
		// 遊戲結束
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
		// 重播

		// 互動遊戲
		InstantEvent = 1000	,

		// 錄音
		RecordEvent	= 1100	,









	};

}}}





#endif
