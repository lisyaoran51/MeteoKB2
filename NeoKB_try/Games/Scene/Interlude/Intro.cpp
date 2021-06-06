#include "Intro.h"

#include "../../../Games/Scene/Select/MainMenu.h"

using namespace Games::Scenes::Interlude;
using namespace Games::Scenes::Select;



Intro::Intro(): RegisterType("Intro")
{
	registerLoad(bind((int(Intro::*)())&Intro::load, this));
}

int Intro::onEntering(Scene * lastScene)
{
	
	//GetScheduler()->AddDelayedTask([=]() { Push(mainMenu); return 0; }, 0.5);

	return MeteoScene::onEntering(lastScene);
}

int Intro::load()
{

	LOG(LogLevel::Info) << "Intro::load() : pushing main menu.";

	mainMenu = new MainMenu();

	Push(mainMenu);

	return 0;
}
