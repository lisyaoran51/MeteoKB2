#ifndef METEO_GAME_H
#define METEO_GAME_H

#include "MeteoGameBase.h"
#include "Scene/Interlude/Intro.h"
#include "../Framework/IO/Storage.h"


using namespace Framework;
using namespace Games::Scenes::Interlude;
using namespace Framework::IO;



namespace Games {


	class MeteoGame : public MeteoGameBase {

		Intro* intro = nullptr;

		Scene* screenStack;

		int load();

		Intro* getIntro();

	public:

		MeteoGame();

		BindablePointer<RulesetInfo*>* GetRuleset();

		virtual Storage* GetStableStorage();

	protected:

		BindablePointer<RulesetInfo*> ruleset;

		virtual int LoadOnComplete();

	};



}







#endif