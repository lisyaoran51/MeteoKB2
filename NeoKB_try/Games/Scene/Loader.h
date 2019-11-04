#ifndef LOADER_H
#define LOADER_H


#include "../../Framework/Scenes/Scene.h"
#include "MeteoScene.h"

using namespace Framework::Scenes;


namespace Games {
namespace Scenes {

	class Loader : public MeteoScene {

		int load();

		int load(MeteoGame* mg);

	public:

		Loader();

	protected:


	};




}
}


#endif