#ifndef METEO_COMMAND_H
#define METEO_COMMAND_H



namespace Games {
namespace Input{
namespace Commands{


	enum class MeteoCommand {
		None = -1				,
		// 識別
		ReadFirmwareVersion = 0	,
		ReturnFirmwareVersion	,
		ReadFirmwareData		,
		ReturnFirmwareData		,
		ReadHardwareData		,
		ReturnHardwareDate		,
		ReadHardwareIdentity	,
		ReturnHardwarIdentity	,
		SendEncryptedIdentifyMessage	,
		ReturnDecryptedIdentifyMessage	,
		FaultIdentity			,
		ReadAppData				,
		ReturnAppData			,
		ReadUserData			,
		ReturnUserData			,
		ChangeUserData			,
		AckChangeUserData		,
		// 系統
		ReadMainboardStatus	= 100,
		ReturnMainboardStatus	,
		RestartMainboard		,
		AckRestartMainboard		,
		RestartControlboard		,
		AckRestartControlboard	,
		RestartApp				,
		AckRestartApp			,
		// 心跳
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
		// 基本功能
		AppIOEvent								,
		KeyboardIOEvent							,

		AppPressPowerButton = 300				,
		AckAppPressPowerButton					,
		PianoPressPowerButton					,
		AckPianoPressPowerButton				,
		AppPressSustainButton					,
		AckAppPressSustainButton				,
		PianoPressSustainButton					,
		AckPianoPressSustainButton				,
		AppPressSensitiveButton					,
		AckAppPressSensitiveButton				,
		PianoPressSensitiveButton				,
		AckPianoPressSensitiveButton			,
		AppPressLowerOctaveButton				,
		AckAppPressLowerOctaveButton			,
		PianoPressLowerOctaveButton				,
		AckPianoPressLowerOctaveButton			,
		AppPressRaiseOctaveButton				,
		AckAppPressRaiseOctaveButton			,
		PianoPressRaiseOctaveButton				,
		AckPianoPressRaiseOctaveButton			,
		AppPressPauseButton						,
		AckAppPressPauseButton					,
		PianoPressPauseButton					,
		AckPianoPressPauseButton				,
		AppAdjustInstrumentVolume				,
		AckAppAdjustInstrumentVolume			,
		PianoAdjustInstrumentVolume				,
		AckPianoAdjustInstrumentVolume			,
		AppAdjustBackgroundButton				,
		AckAppAdjustBackgroundButton			,
		PianoAdjustBackgroundButton				,
		AckPianoAdjustBackgroundButton			,
		AppPluginPedal							,
		AckAppPluginPedal						,
		PianoPluginPedal						,
		AckPianoPluginPedal						,
		AppPluginEarphone						,
		AckAppPluginEarphone					,
		PianoPluginEarphone						,
		AckPianoPluginEarphone					,
		AppPressKey								,
		AckAppPressKey							,
		PianoPressKey							,
		AckPianoPressKey						,
		AppReleaseKey							,
		AckAppReleaseKey						,
		PianoReleaseKey							,
		AckPianoReleaseKey						,
		AppPushInSustainPedal					,
		AckAppPushInSustainPedal				,
		PianoPushInSustainPedal					,
		AckPianoPushInSustainPedal				,
		AppReleaseSustainPedal					,
		AckAppReleaseSustainPedal				,
		PianoReleaseSustainPedal				,
		AckPianoReleaseSustainPedal				,
		AppReadPianoInstruments					,
		AckAppReadPianoInstruments				,
		PianoReadAppInstruments					,
		AckPianoReadAppInstruments				,
		AppSwitchInstrument						,
		AckAppSwitchInstrument					,
		PianoSwitchInstrument					,
		AckPianoSwitchInstrument				,
		FinishAppSwitchInstrument				,
		AckFinishAppSwitchInstrument			,
		FinishPianoSwitchInstrument				,
		AckFinishPianoSwitchInstrument			,
		AppSwitchPianoInstrument				,
		AckAppSwitchPianoInstrument				,
		FinishAppSwitchPianoInstrument			,
		AckFinishAppSwitchPianoInstrument		,
		// 擴充升級
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
		// 設定
		ReadHardwareConfiguration = 600			,
		ReturnHardwareConfiguration				,
		WriteHardwareConfiguration				,
		ReturnWriteHardwareConfiguration		,
		ReadHardwareRecord						,
		ReturnHardwareRecord					,
		// 場警
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
		// 音樂遊戲
		// 遊戲準備
		ReadGameConfiguration = 800				,
		ReturnGameConfiguration					,
		WriteGameConfiguration					,
		ReturnWriteGameConfiguration			,
		SheetmusicData							,
		AckSheetmusicData						,

		RequestSheetmusicFile					,	// 後來佳的
		AckRequestSheetmusicFile				,	// 後來佳的

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
		// 遊戲進行
		MeteorEvent								,

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
		AppQuitGame								,
		AckAppQuitGame							,
		AppRestartGame							,
		AckAppRestartGame						,
		// 遊戲結束
		HardwareFinishGame						,
		AckHardwareFinishGame					,
		FinalScore								,
		AckFinalScore							,
		PlayRecordData							,
		AckPlayRecordData						,

		PostPlayRecord							,
		AckPostPlayRecord						,

		PlayRecordFileSegment					,
		AckPlayRecordFileSegment				,
		RequestRewritePlayRecordFileSegment		,
		RewritePlayRecordFileSegment			,
		AckRewritePlayRecordFileSegment			,
		FinishWritePlayRecord					,
		AckFinishWritePlayRecord				,
		// 重播

		// 互動遊戲
		StartInstant = 1000						,
		AckStartInstant							,
		InstantPressKey							,
		InstantReleaseKey						,
		InstantLedMatrix						,
		InstantLightEvent						,
		InstantPianoEvent						,

		InstantEvent							,

		// 錄音
		RecordEvent								,









	};

}}}





#endif
