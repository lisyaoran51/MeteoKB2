#ifndef METEOR_GAME_H
#define METEOR_GAME_H

#include"../Games/MeteoGame.h"

using namespace Games;

namespace Meteor {



	class MeteorGame : public MeteoGame {

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

		MeteorGame();

		int Run();

	};

}




#endif