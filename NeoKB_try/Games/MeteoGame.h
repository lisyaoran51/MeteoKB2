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

	protected:

		BindablePointer<RulesetInfo*> ruleset;

		OutputManager* outputManager = nullptr;

		virtual int LoadOnComplete();

	};



}







#endif