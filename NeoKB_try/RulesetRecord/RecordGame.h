#ifndef RECORD_GAME_H
#define RECORD_GAME_H

#include"../Games/MeteoGame.h"

using namespace Games;

namespace Record {



	class RecordGame : public MeteoGame {

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

		RecordGame();

		int Run();

	};

}




#endif