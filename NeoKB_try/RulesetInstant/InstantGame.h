#ifndef INSTANT_GAME_H
#define INSTANT_GAME_H

#include"../Games/MeteoGame.h"

using namespace Games;

namespace Instant {



	class InstantGame : public MeteoGame {

		/// <summary>
		/// �o����ӹC��
		/// </summary>
		int load();

		// TODO: ��{����g��thread

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