#include "Intro.h"

#include "../../../Base/Scene/Select/MainMenu.h"

using namespace Base::Scenes::Interlude;
using namespace Base::Scenes::Select;



int Intro::onEntering(Scene * lastScene)
{
	mainMenu = new MainMenu();

	GetScheduler()->AddDelayedTask([=]() { Push(mainMenu); return 0; }, 500);

	return 0;
}

int Intro::load()
{


	return 0;
}
