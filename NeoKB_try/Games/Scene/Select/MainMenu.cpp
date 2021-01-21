#include "MainMenu.h"

#include "PlaySongSelect.h"

using namespace Games::Scenes::Select;

MainMenu::MainMenu(): RegisterType("MainMenu")
{
}

int MainMenu::onEntering(Scene * lastScene)
{
	MeteoScene::onEntering(lastScene);

	songSelect = new PlaySongSelect();

	Push(songSelect);
	//GetScheduler()->AddDelayedTask([=]() { Push(songSelect); return 0; }, 0.5);

	return 0;
}

int MainMenu::load()
{
	return 0;
}
