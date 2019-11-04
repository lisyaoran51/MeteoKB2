#include "Loader.h"

#include "Interlude/Intro.h" 

using namespace Framework::Scenes;
using namespace Games::Scenes;
using namespace Games::Scenes::Interlude;


int Loader::load()
{
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
