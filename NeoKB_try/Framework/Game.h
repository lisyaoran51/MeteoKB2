#ifndef GAME_H
#define GAME_H


#include "Allocation/Hierachal/Container.h"
#include "IO/Stores/ResourceStore.h"
#include "Audio/AudioManager.h"
#include "Host/GameHost.h"


using namespace Framework::Allocation;
using namespace Framework::IO;
using namespace Framework::Audio;
using namespace Framework::Host;


namespace Framework {

	class GmeHost;

	class Game : public Container {
		
		AudioManager* audioManager;
		
		ResourceStore<string>* resources;
		
		GameHost* gameHost;
		
		int load();

	public:

		
		
		virtual int SetHost(GameHost* host);
		
		virtual string GetResourcePath();
		
	protected:
		
		Game();
		
	};

}







#endif