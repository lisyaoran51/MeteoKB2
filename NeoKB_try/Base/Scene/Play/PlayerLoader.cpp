#include "PlayerLoader.h"


using namespace Base::Scenes::Play;


PlayerLoader::PlayerLoader(Player * p) : RegisterType("PlayerLoader"), MeteoScene()
{
	registerLoad(bind((int(PlayerLoader::*)())&PlayerLoader::load, this));
}

int PlayerLoader::load()
{
	LoadAsync(player);

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
