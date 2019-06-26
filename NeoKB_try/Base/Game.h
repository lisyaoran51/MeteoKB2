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
		/// �o����ӹC��
		/// </summary>
		int load();

		// TODO: ��{����g��thread

		// update thread

		// draw thread

		// audio thread

		// input thread

		Updater* updater;

		/// <summary>
		/// �o��handler�O�b�C���}�l�e�A�brun�άOsetup config�ɧ�openTK��handler�\�i�h
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