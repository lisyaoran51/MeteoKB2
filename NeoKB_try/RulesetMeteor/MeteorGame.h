#ifndef METEOR_GAME_H
#define METEOR_GAME_H

#include"../Base/MeteoGame.h"

using namespace Base;

namespace Meteor {



	class MeteorGame : public MeteoGame {

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

		MeteorGame();

		int Run();

	};

}




#endif