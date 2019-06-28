#ifndef GAME_H
#define GAME_H

#include "Play/Player.h"
#include "../Framework/Allocation/Hierachal/Triggerable.h"
#include "../Util/Update/Updater.h"
#include "../Util/Hierachal/ChildAddable.h"
#include "../Framework/Threading/GameThread.h"

using namespace Framework::Allocation::Hierachal;
using namespace Base::Play;
using namespace Framework::Threading;


namespace Base {



	class Game: public Triggerable {

		/// <summary>
		/// �o����ӹC��
		/// </summary>
		int load();

		// TODO: ��{����g��thread


	public:

		Game();

		int Run();

	protected:

		Player* player;

	};

}




#endif