#include "Loader.h"

using namespace Framework::Scenes;
using namespace Games::Scenes;



int Loader::load()
{
	return 0;
}

Loader::Loader(): RegisterType("Loader")
{
	registerLoad(bind((int(Loader::*)())&Loader::load, this));
	isValidForResume = false;
}
