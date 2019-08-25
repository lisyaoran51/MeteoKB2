#include "Loader.h"

using namespace Framework::Scenes;
using namespace Base::Scenes;



int Loader::load()
{
	return 0;
}

Loader::Loader(): RegisterType("Loader"), Scene()
{
	registerLoad(bind((int(Loader::*)())&Loader::load, this));
	isValidForResume = false;
}
