#ifndef PLAYER_LOADER_H
#define PLAYER_LOADER_H


#include "../../../Framework/Scenes/Scene.h"
#include "Player.h"
#include "../MeteoScene.h"


using namespace Framework::Scenes;
using namespace Framework::Allocation::Hierachal;
using namespace Games::Scenes;


namespace Games {
namespace Scenes {
namespace Play {

	class PlayerLoader: public MeteoScene {

		Player* player;

		int onPlayerRestartRequest();

	public:

		PlayerLoader(Player* p);

	protected:


	private:

		int load();

		virtual int onResuming(Scene* lastScene);

		/// <summary>
		/// 不停詢問是否讀取完成，完成後進入player畫面
		/// </summary>
		int pushWhenLoaded();

	};

}}}






#endif
