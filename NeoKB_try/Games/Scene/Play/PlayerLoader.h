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
		/// �����߰ݬO�_Ū�������A������i�Jplayer�e��
		/// </summary>
		int pushWhenLoaded();

	};

}}}






#endif
