#include "Base.h"

using namespace Framework;
using namespace Framework::Allocation;
using namespace Framework::IO;
using namespace Framework::Audio;
using namespace Framework::Host;


int Base::load()
{
	resources = new ResourceStore<string>();
	resources->AddStore(new DynamicPathResourceStore(GetResourcePath()));


	auto tracks = new NamespacedResourceStore<string>(resources, "Tracks");
	auto samples = new NamespacedResourceStore<string>(resources, "Samples");

	audioManager = new AudioManager(tracks, samples);

	Cache<AudioManager>(audioManager);

	return 0;
}

Base::Base() : RegisterType("Base")
{
}

int Base::SetHost(GameHost * host)
{
	gameHost = host;
	return 0;
}

string Base::GetResourcePath()
{
	return string("Resources");
}
