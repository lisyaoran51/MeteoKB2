#ifndef LOADER_H
#define LOADER_H


#include "../../Framework/Scenes/Scene.h"
#include "MeteoScene.h"

using namespace Framework::Scenes;


namespace Games {
namespace Scenes {

	class Loader : public MeteoScene {

		int load();

	public:

		Loader();

	protected:


	};




}
}


#endif