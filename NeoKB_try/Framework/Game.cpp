#include "Game.h"

#include "IO/Stores/NamespacedResourceStore.h"
#include "IO/Stores/DynamicPathResourceStore.h"
#include "../Util/Log.h"

using namespace Framework;
using namespace Framework::IO::Stores;
using namespace Util;

int Game::load()
{
	return 0;
}

Game::Game() : RegisterType("Game")
{
	registerLoad(bind((int(Game::*)())&Game::load, this));
	//CommandInputManager<FrameworkAction, BluetoothCommand>();
}
