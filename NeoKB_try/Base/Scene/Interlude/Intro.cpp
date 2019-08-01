#include "Intro.h"

using namespace Base::Scene::Interlude;



int Intro::onEntering(Scene * lastScene)
{
	mainMenu = new MainMenu();

	GetScheduler()->AddDelayedTask([]() { Push(mainMenu); return 0; }, 500);

	return 0;
}

int Intro::load()
{


	return 0;
}
