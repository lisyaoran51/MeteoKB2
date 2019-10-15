#ifndef METEO_GAME_H
#define METEO_GAME_H

#include "MeteoGameBase.h"
#include "Scene\Interlude\Intro.h"


using namespace Framework;
using namespace Games::Scenes::Interlude;

namespace Games {


	class MeteoGame : public MeteoGameBase {

		Intro* intro;

		Scene* screenStack;

		int load();

	public:

		MeteoGame();

		Bindable<RulesetInfo*>* GetRuleset();

	protected:

		Bindable<RulesetInfo*> ruleset;

		virtual int LoadOnComplete();

	};



}







#endif