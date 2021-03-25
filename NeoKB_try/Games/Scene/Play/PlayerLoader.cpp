#include "PlayerLoader.h"

#include "../../../Framework/Threading/ThreadMaster.h"


using namespace Games::Scenes::Play;


int PlayerLoader::onPlayerRestartRequest()
{
	SetIsValidForResume(true);

	return 0;
}

PlayerLoader::PlayerLoader(Player * p) : RegisterType("PlayerLoader"), MeteoScene()
{
	player = p;
	registerLoad(bind((int(PlayerLoader::*)())&PlayerLoader::load, this));
}

int PlayerLoader::load()
{
	LOG(LogLevel::Info) << "PlayerLoader::load() : push player " << player;

	// 這裡原本是用load async，他可以在背景執行add child，不過我們不想增加thread，所以就拿掉了，直接用add child
	// 不過這樣push when loaded就沒用了
	AddChild(player);
	player->AddOnRestartRequested(this, bind((int(PlayerLoader::*)())&PlayerLoader::onPlayerRestartRequest, this), "PlayerLoader::onPlayerRestartRequest");

	pushWhenLoaded();

	return 0;
}

int PlayerLoader::onResuming(Scene * lastScene)
{
	// TODO: 這邊要做thread safe
	/*
	DeleteChild(player);
	ThreadMaster::GetInstance().AddObjectToDelete(player);
	*/
	player = nullptr;

	GetScheduler()->AddDelayedTask([=] {
		
		player = new Player();

		AddChild(player);
		player->AddOnRestartRequested(this, bind((int(PlayerLoader::*)())&PlayerLoader::onPlayerRestartRequest, this), "PlayerLoader::onPlayerRestartRequest");

		pushWhenLoaded();

		return 0;
	}, 0.5);
	
	return MeteoScene::onResuming(lastScene);
}

int PlayerLoader::pushWhenLoaded()
{
	if (player->GetLoadState() != LoadState::Ready) {
		try {
			GetScheduler()->AddDelayedTask(bind((int(PlayerLoader::*)())&PlayerLoader::pushWhenLoaded, this), 0.5);
		}
		catch (exception& e) {
			LOG(LogLevel::Error) << "PlayerLoader::pushWhenLoaded() : add delay task failed [" << e.what() << "].";
			abort();
		}

		return -1;
	}

	Push(player);

	SetIsValidForResume(false);

	return 0;
}
