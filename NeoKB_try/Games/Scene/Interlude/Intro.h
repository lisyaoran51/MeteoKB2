#ifndef INTRO_H
#define INTRO_H

#include "../../../Framework/Scenes/Scene.h"
#include "../Select/MainMenu.h"
#include "../MeteoScene.h"

using namespace Framework::Scenes;
using namespace Games::Scenes::Select;
using namespace Games::Scenes;

namespace Games {
namespace Scenes {
namespace Interlude {


	class Intro : public MeteoScene {

	public:

		Intro();

	protected:

		virtual int onEntering(Scene* lastScene);

	private:

		MainMenu* mainMenu = nullptr;

		/// <summary>
		/// Ū��
		/// AudioManager audio, OsuConfigManager config, BeatmapManager beatmaps, Framework.Game game
		/// ���JAddDelayed song select����
		/// </summary>
		int load();

	};

}}}






#endif
