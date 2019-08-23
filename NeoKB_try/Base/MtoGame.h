#ifndef MTO_GAME_H
#define MTO_GAME_H

#include "MtoGameBase.h"
#include "Scene\Interlude\Intro.h"


using namespace Framework;
using namespace Base::Scenes::Interlude;

namespace Base {


	class MtoGame : public MtoGameBase {

		Intro* intro;

		Scene* screenStack;

		int load();

	public:

		MtoGame();

	protected:

		virtual int LoadOnComplete();

	};



}







#endif