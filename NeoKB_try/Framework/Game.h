#ifndef GAME_H
#define GAME_H


#include "Base.h"
#include "IO/Stores/ResourceStore.h"
#include "Audio/AudioManager.h"
#include "Host/GameHost.h"


using namespace Framework::Allocation;
using namespace Framework::IO;
using namespace Framework::Audio;
using namespace Framework::Host;


namespace Framework {

	namespace Host {

		//class GameHost;

	}

	class Game : public Base {
		
		int load();

	public:

		
		
		//virtual string GetResourcePath();
		
	protected:
		
		Game();

		AudioManager* audioManager;

		//ResourceStore<string>* resources;
		
	};

}







#endif