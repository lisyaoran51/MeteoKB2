#include "Base.h"


#include "IO/Stores/NamespacedResourceStore.h"
#include "IO/Stores/DynamicPathResourceStore.h"

using namespace Framework;
using namespace Framework::Allocation;
using namespace Framework::IO;
using namespace Framework::Audio;
using namespace Framework::Host;
using namespace Framework::IO::Stores;



int Base::load()
{
	LOG(LogLevel::Info) << "Base::load() : caching audio manager and main resource.";

	resources = new CompositeResourceStore<char*>();
	resources->AddStore(new DynamicPathResourceStore(GetResourcePath()));


	auto tracks = new NamespacedResourceStore<char*>(resources, "Tracks");
	auto samples = new NamespacedResourceStore<char*>(resources, "Samples");

	audioManager = new AudioManager(tracks, samples);

	Cache<AudioManager>(audioManager);

	return 0;
}

Base::Base() : RegisterType("Base")
{
	registerLoad(bind((int(Base::*)())&Base::load, this));
}

int Base::SetHost(GameHost * host)
{
	LOG(LogLevel::Debug) << "Base::SetHost() : host address = " << host;

	gameHost = host;
	return 0;
}

string Base::GetResourcePath()
{
	return string("Resources");
}
