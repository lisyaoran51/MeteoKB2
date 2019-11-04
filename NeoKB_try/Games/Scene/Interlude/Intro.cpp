#include "Intro.h"

#include "../../../Games/Scene/Select/MainMenu.h"

using namespace Games::Scenes::Interlude;
using namespace Games::Scenes::Select;



int Intro::onEntering(Scene * lastScene)
{
	mainMenu = new MainMenu();

	GetScheduler()->AddDelayedTask([=]() { Push(mainMenu); return 0; }, 0.5);

	return 0;
}

int Intro::load()
{


	return 0;
}
