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
		/// 不停詢問是否讀取完成，完成後進入player畫面
		/// </summary>
		int pushWhenLoaded();

	};

}}}






#endif
