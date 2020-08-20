#ifndef INSTANT_GAME_H
#define INSTANT_GAME_H

#include"../Games/MeteoGame.h"

using namespace Games;

namespace Instant {



	class InstantGame : public MeteoGame {

		/// <summary>
		/// 這邊把整個遊戲
		/// </summary>
		int load();

		// TODO: 把程式改寫成thread

		// update thread

		// draw thread

		// audio thread

		// input thread

	public:

		InstantGame();

		int Run();

	};

}




#endif