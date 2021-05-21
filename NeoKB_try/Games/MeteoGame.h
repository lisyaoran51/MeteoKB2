#ifndef METEO_GAME_H
#define METEO_GAME_H

#include "MeteoGameBase.h"
#include "Scene/Interlude/Intro.h"
#include "../Framework/IO/Storage.h"
#include "UI/VolumeController.h"


using namespace Framework;
using namespace Games::Scenes::Interlude;
using namespace Framework::IO;
using namespace Games::UI;



namespace Games {


	class MeteoGame : public MeteoGameBase {

		Intro* intro = nullptr;

		Scene* screenStack;

		int load();

		int load(OutputManager* o);

		Intro* getIntro();

		VolumeController* volumeController = nullptr;

	public:

		MeteoGame();

		BindablePointer<RulesetInfo*>* GetRuleset();

		virtual Storage* GetStableStorage();

		/// <summary>
		/// 應該只有bluetooth裝置可以call這個函式
		/// </summary>
		virtual int TriggerOnConnect();

		/// <summary>
		/// 應該只有bluetooth裝置可以call這個函式
		/// </summary>
		virtual int TriggerOnDisconnect();

		/// <summary>
		/// 在連線時會執行的事情
		/// </summary>
		virtual int SetConnectState();

		/// <summary>
		/// 在斷線時會執行的事情
		/// </summary>
		virtual int SetDisconnectState();

	protected:

		BindablePointer<RulesetInfo*> ruleset;

		OutputManager* outputManager = nullptr;

		bool isBluetoothConnected = false;

		virtual int LoadOnComplete();

		int onConnect();

		int onDisconnect();

	};



}







#endif