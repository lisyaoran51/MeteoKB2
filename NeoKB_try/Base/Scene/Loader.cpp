#include "Loader.h"

using namespace Base::Scene;




int Loader::load()
{
	return 0;
}

Loader::Loader(): RegisterType("Loader"), Scene()
{
	registerLoad(bind((int(Loader::*)())&Loader::load, this));
	isValidForResume = false;
}
