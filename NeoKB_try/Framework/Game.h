#ifndef GAME_H
#define GAME_H


#include "Base.h"
#include "IO/Stores/ResourceStore.h"
#include "Audio/AudioManager.h"
#include "Hosts/Host.h"
#include "Input/UserInputManager.h"
#include "Input/Messages/MessageInputManager.h"


using namespace Framework::Allocation;
using namespace Framework::IO;
using namespace Framework::Audio;
using namespace Framework::Hosts;
using namespace Framework::Input;
using namespace Framework::Input::Messages;



namespace Framework {

	namespace Hosts {

		//class Host;

	}

	class Game : public Base {
		
		int load();

	public:

		virtual InputManager* CreateInputManager() = 0;
		
		//virtual string GetResourcePath();
		
	protected:
		
		Game();


		//ResourceStore<string>* resources;
		
	};

}







#endif