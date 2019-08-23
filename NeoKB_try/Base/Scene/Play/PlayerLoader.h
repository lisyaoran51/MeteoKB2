#ifndef PLAYER_LOADER_H
#define PLAYER_LOADER_H


#include "../../../Framework/Scenes/Scene.h"
#include "Player.h"


using namespace Framework::Scenes;
using namespace Framework::Allocation::Hierachal;

namespace Base {
namespace Scenes {
namespace Play {

	class PlayerLoader: public Scene {

		Player* player;

	public:

		PlayerLoader(Player* p);

	protected:


	private:

		int load();

		/// <summary>
		/// �����߰ݬO�_Ū�������A������i�Jplayer�e��
		/// </summary>
		int pushWhenLoaded();

	};

}}}






#endif
