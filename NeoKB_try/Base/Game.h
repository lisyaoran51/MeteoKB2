#ifndef GAME_H
#define GAME_H

#include "Play/Player.h"
#include "../Util/Update/Updater.h"
#include "../Util/Hierachal/ChildAddable.h"

using namespace Util::Hierachal;
using namespace Base::Play;
using namespace Util::Update;


namespace Base {



	class Game: public ChildAddable {

		/// <summary>
		/// 這邊把整個遊戲
		/// </summary>
		int load();

		// TODO: 把程式改寫成thread

		// update thread

		// draw thread

		// audio thread

		// input thread

		Updater* updater;

		/// <summary>
		/// 這些handler是在遊戲開始前，在run或是setup config時把openTK的handler擺進去
		/// </summary>
		vector<InputHandler*> availableInputHandlers;

	public:

		Game();

		int Run();

	protected:

		Player* player;

	};

}




#endif