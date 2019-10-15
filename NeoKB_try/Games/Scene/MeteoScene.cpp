#include "MeteoScene.h"


using namespace Games::Scenes;



int MeteoScene::load()
{
	MeteoGame * game = GetCache<MeteoGame>("MeteoGame");
	if (!f)
		throw runtime_error("int  MeteoScene<T>::load() : MeteoGame not found in cache.");

	MeteoGameBase * gameBase = GetCache<MeteoGameBase>("MeteoGameBase");
	if (!f)
		throw runtime_error("int  MeteoScene<T>::load() : MeteoGameBase not found in cache.");


	return load(game, gameBase);
}

int MeteoScene::load(MeteoGame * game, MeteoGameBase * gameBase)
{

	rulesetInfo.BindTo(game->GetRuleset());
	workingSm.BindTo(gameBase->GetWorkingSm());
	

	return 0;
}

MeteoScene::MeteoScene() : RegisterType("MeteoScene"), Scene()
{
	registerLoad(bind((int(MeteoScene::*)())&MeteoScene::load, this));
}

MeteoScene::~MeteoScene()
{
	rulesetInfo.Unbind();
	workingSm.Unbind();

}
