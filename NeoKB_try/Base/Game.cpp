#include"Game.h"

#include"Config/FrameworkConfigManager.h"
#include"../Util/Update/Updater.h"
#include "../Util/Log.h"

using namespace Base;
using namespace Base::Config;
using namespace Util::Update;
using namespace Util::Hierachal;
using namespace Util;


int Game::load()
{
	LOG(LogLevel::Info) << "Game::load : start loading the game and building updater";

	// FrameworkConfigManager不該自己產生，應該是從外部cache
	// Cache<FrameworkConfigManager>(new FrameworkConfigManager());
	Cache<Updater>(updater = new Updater());

	// 先選歌
	// Cache 哥曲和規則資料
	// TODO:....

	player = new Player();

	AddChild(player);

	return 0;
}

Game::Game(): RegisterType("Game"), ChildAddable()
{
	registerLoad(bind((int(Game::*)())&Game::load, this));
}

int Game::Run()
{
	bool running = true;
	while (running) {
		updater->Update();
	}

	return 0;
}
