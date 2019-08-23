#ifndef MTO_GAME_BASE_H
#define MTO_GAME_BASE_H

#include "../Framework/Game.h"
#include "Scene\Interlude\Intro.h"


using namespace Framework;
using namespace Framework::Scene;
using namespace Base::Scene::Interlude;

namespace Base {


	class MtoGameBase : public Game {


		int load();

	public:

		MtoGameBase();

	protected:

		virtual int LoadOnComplete();

	};



}







#endif