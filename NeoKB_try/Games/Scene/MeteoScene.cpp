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
	LOG(LogLevel::Debug) << "MeteoScene::~MeteoScene() : killing scene [" << GetTypeName() << "].";
	rulesetInfo.DeleteOnValueChanged(this);
	workingSm.DeleteOnValueChanged(this);

	rulesetInfo.UnBind();
	workingSm.UnBind();

}

int MeteoScene::onExpire()
{
	Scene::onExpire();

	isMessageActive = false;

	LOG(LogLevel::Debug) << "MeteoScene::onExpire() : expiring [" << GetTypeName() << "].";

	return 0;
}

int MeteoScene::onEntering(Scene * lastScene)
{
	LOG(LogLevel::Debug) << "MeteoScene::onEntering() : entering [" << GetTypeName() << "].";
	isMessageActive = true;

	GetScheduler()->AddTask([=]() {

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::EnterScene);
		json returnContext;

		returnContext["Scene"] = GetTypeName();
		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);
	
		return 0;
	});

	return Scene::onEntering(lastScene);
}

int MeteoScene::onExiting(Scene * lastScene)
{
	LOG(LogLevel::Debug) << "MeteoScene::onExiting() : exiting [" << GetTypeName() << "].";
	isMessageActive = false;

	return Scene::onExiting(lastScene);
}

int MeteoScene::onSuspending(Scene * lastScene)
{
	return Scene::onSuspending(lastScene);
}

int MeteoScene::onResuming(Scene * lastScene)
{
	LOG(LogLevel::Debug) << "MeteoScene::onResuming() : resuming [" << GetTypeName() << "].";
	isMessageActive = true;

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

	LOG(LogLevel::Debug) << "MeteoScene::onMessage() : got new bt message. " << GetTypeName();

	if (contextMessage->GetCommand() == MeteoCommand::ReadScene) {
		LOG(LogLevel::Debug) << "MeteoScene::onMessage() : got new bt message [ReadScene]. ";

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::ReturnScene);
		json returnContext;

		returnContext["Scene"] = GetTypeName();
		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);
	}

	if (contextMessage->GetCommand() == MeteoCommand::EnterScene) {
		LOG(LogLevel::Debug) << "MeteoScene::onMessage() : got new bt message [EnterScene]. ";

		string sceneName = contextMessage->GetContextInJson()["Scene"].get<string>();


		Scene* targetScene = nullptr;
		for (Scene* s = this; s != nullptr; s = s->GetParentScene()) {

			if (sceneName == s->GetTypeName()) {

				targetScene = s;
				break;

			}
		}

		if (targetScene == nullptr) {
			MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckEnterScene);
			json returnContext;

			returnContext["Status"] = -1;
			meteoContextBluetoothMessage->SetContextInJson(returnContext);
			meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

			outputManager->PushMessage(meteoContextBluetoothMessage);

			return 0;
		}

		for (Scene* s = this; s->GetParentScene() != targetScene; s = s->GetParentScene()) {

			s->SetIsValidForResume(false);

		}
		targetScene->SetIsValidForResume(true);

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::AckEnterScene);
		json returnContext;

		returnContext["Status"] = 0;
		meteoContextBluetoothMessage->SetContextInJson(returnContext);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);

		Exit();

	}


	return 0;
}
