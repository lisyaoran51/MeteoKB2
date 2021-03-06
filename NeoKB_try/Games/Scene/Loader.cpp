#include "Loader.h"

#include "Interlude/Intro.h" 
#include "../MeteoGame.h"

using namespace Framework::Scenes;
using namespace Games::Scenes;
using namespace Games::Scenes::Interlude;
using namespace Games;


int Loader::load()
{
	LOG(LogLevel::Info) << "Loader::load() : pushing intro scene.";

	MeteoGame* mg = GetDependencies()->GetCache<MeteoGame>("MeteoGame");

	return load(mg);
}

int Loader::load(MeteoGame * mg)
{
	Push(new Intro());
	return 0;
}

Loader::Loader(): RegisterType("Loader")
{
	registerLoad(bind((int(Loader::*)())&Loader::load, this));
	isValidForResume = false;
}
