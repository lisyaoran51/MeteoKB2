#include "PlayerLoader.h"


using namespace Base::Scenes::Play;


PlayerLoader::PlayerLoader(Player * p) : RegisterType("PlayerLoader"), MeteoScene()
{
	registerLoad(bind((int(PlayerLoader::*)())&PlayerLoader::load, this));
}

int PlayerLoader::load()
{
	// 這裡原本是用load async，他可以在背景執行add child，不過我們不想增加thread，所以就拿掉了，直接用add child
	// 不過這樣push when loaded就沒用了
	AddChild(player);

	pushWhenLoaded();

	return 0;
}

int PlayerLoader::pushWhenLoaded()
{
	if (player->GetLoadState() != LoadState::Ready) {
		GetScheduler()->AddDelayedTask(bind((int(PlayerLoader::*)())&PlayerLoader::pushWhenLoaded, this), 500);
		return -1;
	}

	Push(player);

	SetIsValidForResume(false);

	return 0;
}
