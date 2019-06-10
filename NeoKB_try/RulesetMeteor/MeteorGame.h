#ifndef METEOR_GAME_H
#define METEOR_GAME_H

#include"../Base/Game.h"

using namespace Base;

namespace Meteor {



	class MeteorGame : public Game {

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