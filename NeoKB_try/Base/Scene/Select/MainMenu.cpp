#include "MainMenu.h"

#include "PlaySongSelect.h"

using namespace Base::Scenes::Select;

int MainMenu::onEntering(Scene * lastScene)
{

	songSelect = new PlaySongSelect();

	GetScheduler()->AddDelayedTask([=]() { Push(songSelect); return 0; }, 500);

	return 0;
}

int MainMenu::load()
{
	return 0;
}
