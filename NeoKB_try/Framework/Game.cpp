#include "Game.h"

#include "IO\Stores\NamespacedResourceStore.h"
#include "IO\Stores\DynamicPathResourceStore.h"

using namespace Framework;
using namespace Framework::IO::Stores;

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

string Game::GetResourcePath()
{
	return string("Resources");
}

Game::Game() : RegisterType("Game")
{
}
