#ifndef METEO_GAME_BASE_H
#define METEO_GAME_BASE_H

#include "../Framework/Game.h"
#include "Scene\Interlude\Intro.h"


using namespace Framework;
using namespace Framework::Scenes;
using namespace Base::Scenes::Interlude;

namespace Base {


	class MeteoGameBase : public Game {


		int load();

	public:

		MeteoGameBase();

		Bindable<WorkingSm*>* GetWorkingSm();

	protected:

		Bindable<WorkingSm*> workingSm;

		virtual int LoadOnComplete();

	};



}







#endif