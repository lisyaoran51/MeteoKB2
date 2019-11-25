#include "MeteoScene.h"

#include "../MeteoGame.h"
#include "../MeteoGameBase.h"


using namespace Games::Scenes;
using namespace Games;



int MeteoScene::load()
{
	MeteoGame * game = GetCache<MeteoGame>("MeteoGame");
	if (!game)
		throw runtime_error("int  MeteoScene<T>::load() : MeteoGame not found in cache.");

	MeteoGameBase * gameBase = GetCache<MeteoGameBase>("MeteoGameBase");
	if (!gameBase)
		throw runtime_error("int  MeteoScene<T>::load() : MeteoGameBase not found in cache.");


	return load(game, gameBase);
}

int MeteoScene::load(MeteoGame * game, MeteoGameBase * gameBase)
{
	LOG(LogLevel::Info) << "MeteoScene::load() : binding rulesets and working sm to game.";

	rulesetInfo.BindTo(game->GetRuleset());
	workingSm.BindTo(gameBase->GetWorkingSm());
	

	return 0;
}

BindablePointer<WorkingSm*>* MeteoScene::GetWorkingSm()
{
	return &workingSm;
}

MeteoScene::MeteoScene() : RegisterType("MeteoScene"), Scene()
{
	registerLoad(bind((int(MeteoScene::*)())&MeteoScene::load, this));
}

MeteoScene::~MeteoScene()
{
	rulesetInfo.UnbindAll();
	workingSm.UnbindAll();

}
