#ifndef SOUND_SELECT_PANEL_H
#define SOUND_SELECT_PANEL_H

#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../Sheetmusic/SheetmusicInfo.h"
#include "../Sheetmusic/SheetmusicManager.h"
#include "../../Framework/Input/Messages/MessageHandler.h"




using namespace Framework::Allocation::Hierachal;
using namespace Games::Sheetmusics;
using namespace Framework::Input::Messages;


namespace Games {
namespace UI {

	/// <summary>
	/// 在這邊選則歌曲，之後ispresent應該要改成註冊到song select的scene enter和resume上，不要自己去設定ispresent
	/// </summary>
	class SoundSelectPanel : public Container, public MessageHandler<MeteoBluetoothMessage> {

		int load();

		int load(FrameworkConfigManager* f, Instrument* i, AudioManager* a, OutputManager* o);

	public:

		SoundSelectPanel();

		function<int(void)> StartDownload;

		function<int(void)> TerminateDownload;

		function<int(void)> OnDownloaded;

		function<int(void)> StartSwitchSound;

		function<int(void)> OnSoundSwitched;

		virtual int OnMessage(MeteoBluetoothMessage* command);

		virtual int update();

	protected:

		Instrument* instrument = nullptr;

		FrameworkConfigManager* frameworkConfigManager = nullptr;

		AudioManager* audioManager = nullptr;

		OutputManager* outputManager = nullptr;

	};

}}




#endif