#include "Game.h"

#include "IO/Stores/NamespacedResourceStore.h"
#include "IO/Stores/DynamicPathResourceStore.h"
#include "../Util/Log.h"

using namespace Framework;
using namespace Framework::IO::Stores;
using namespace Util;

int Game::load()
{
	

	audioManager = GetDependencies()->GetCache<AudioManager>("AudioManager");


	return 0;
}

int Game::SetHost(GameHost * host)
{
	gameHost = host;
	return 0;
}

Game::Game() : RegisterType("Game")
{
}
