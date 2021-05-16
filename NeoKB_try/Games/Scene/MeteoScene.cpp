#include "MeteoScene.h"

#include "../MeteoGame.h"
#include "../MeteoGameBase.h"
#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"


using namespace Games::Scenes;
using namespace Games;
using namespace Games::Output::Bluetooths;



int MeteoScene::load()
{
	MeteoGame * game = GetCache<MeteoGame>("MeteoGame");
	if (!game)
		throw runtime_error("int  MeteoScene<T>::load() : MeteoGame not found in cache.");

	MeteoGameBase * gameBase = GetCache<MeteoGameBase>("MeteoGameBase");
	if (!gameBase)
		throw runtime_error("int  MeteoScene<T>::load() : MeteoGameBase not found in cache.");

	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("int  MeteoScene<T>::load() : OutputManager not found in cache.");


	return load(game, gameBase, o);
}

int MeteoScene::load(MeteoGame * game, MeteoGameBase * gameBase, OutputManager* o)
{
	LOG(LogLevel::Info) << "MeteoScene::load() : binding rulesets and working sm to game.";

	rulesetInfo.BindTo(game->GetRuleset());
	workingSm.BindTo(gameBase->GetWorkingSm());
	outputManager = o;
	

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
	rulesetInfo.DeleteOnValueChanged(this);
	workingSm.DeleteOnValueChanged(this);

	rulesetInfo.UnBind();
	workingSm.UnBind();

}

int MeteoScene::onEntering(Scene * lastScene)
{

	MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::EnterScene);
	json returnContext;

	returnContext["Scene"] = GetTypeName();
	meteoContextBluetoothMessage->SetContextInJson(returnContext);
	meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

	outputManager->PushMessage(meteoContextBluetoothMessage);

	return Scene::onEntering(lastScene);
}

int MeteoScene::onExiting(Scene * lastScene)
{
	return Scene::onExiting(lastScene);
}

int MeteoScene::onSuspending(Scene * lastScene)
{
	return Scene::onSuspending(lastScene);
}

int MeteoScene::onResuming(Scene * lastScene)
{
	MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::EnterScene);
	json returnContext;

	returnContext["Scene"] = GetTypeName();
	meteoContextBluetoothMessage->SetContextInJson(returnContext);
	meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

	outputManager->PushMessage(meteoContextBluetoothMessage);

	return Scene::onResuming(lastScene);
}

int MeteoScene::onMessage(MeteoBluetoothMessage * message)
{
	MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);

	if (contextMessage == nullptr) {
		return -1;
	}

	LOG(LogLevel::Debug) << "MeteoScene::onMessage() : got new bt message. ";

	if (contextMessage->GetCommand() == MeteoCommand::ReadScene) {

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::ReturnScene);
		json returnContext;

		returnContext["Scene"] = GetTypeName();
		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);
	}


	return 0;
}
