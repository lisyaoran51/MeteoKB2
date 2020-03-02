#ifndef METEO_GAME_BASE_H
#define METEO_GAME_BASE_H

#include "../Framework/Game.h"
#include "Scene/Interlude/Intro.h"
#include "Ruleset/RulesetStore.h"
#include "../Framework/IO/FileStore.h"
#include "../Framework/Input/KeyBindings/KeyBindingStore.h"
#include "../Framework/Database/DatabaseContextFactory.h"
#include "Configurations/MeteoConfigManager.h"
#include "../Util/DataStructure/BindablePointer.h"


using namespace Framework;
using namespace Framework::Scenes;
using namespace Games::Scenes::Interlude;
using namespace Framework::Input::KeyBindings;
using namespace Framework::IO;
using namespace Framework::Database;
using namespace Games::Configurations;
using namespace Util::DataStructure;


namespace Games {


	class MeteoGameBase : public Game {

		DatabaseContextFactory* dbContextFactory;

		int load();

	public:

		MeteoGameBase();

		BindablePointer<WorkingSm*>* GetWorkingSm();

		virtual int SetHost(GameHost* host);

	protected:

		RulesetStore* rulesetStore;

		FileStore* fileStore;

		KeyBindingStore* keyBindingStore;

		SmManager* smManager;

		BindablePointer<WorkingSm*> workingSm;

		MeteoConfigManager* localConfig;

		virtual int update();

	private:

		int onWorkingSmValueChanged(void* wSm);
	};



}







#endif