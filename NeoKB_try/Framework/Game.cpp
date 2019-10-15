#include "Game.h"

#include "IO\Stores\NamespacedResourceStore.h"
#include "IO\Stores\DynamicPathResourceStore.h"

using namespace Framework;
using namespace Framework::IO::Stores;

int Game::load()
{
	resources = new ResourceStore<string>();
	resources->AddStore(new DynamicPathResourceStore(GetResourcePath()));


	auto tracks = new NamespacedResourceStore<string>(resources, "Tracks");
	auto samples = new NamespacedResourceStore<string>(resources, "Samples");

	audioManager = new AudioManager(tracks, samples);

	Cache<AudioManager>(audioManager);


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
