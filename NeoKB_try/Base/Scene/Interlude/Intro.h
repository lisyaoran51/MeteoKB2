#ifndef INTRO_H
#define INTRO_H

#include "../../../Framework/Scenes/Scene.h"

using namespace Framework::Scene;

namespace Base {
namespace Scene {
namespace Interlude {


	class Intro : public Scene {

	public:



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
