#include "MeteoGameBase.h"

#include <functional>


using namespace Games;
using namespace Games::Rulesets;
using namespace std;


int MeteoGameBase::load()
{

	dbContextFactory = new DatabaseContextFactory(gameHost);

	GetDependencies()->Cache<MeteoGameBase>(this, "MeteoGameBase");

	GetDependencies()->Cache<RulesetStore>(rulesetStore = new RulesetStore(bind(&DatabaseContextFactory::GetContext, dbContextFactory)));

	GetDependencies()->Cache<FileStore>(fileStore = new FileStore(bind(&DatabaseContextFactory::GetContext, dbContextFactory), gameHost->GetStorage()));

	GetDependencies()->Cache<KeyBindingStore>(keyBindingStore = new KeyBindingStore(bind(&DatabaseContextFactory::GetContext, dbContextFactory)));

	GetDependencies()->Cache<SmManager>(smManager = new SmManager(gameHost->GetStorage(), bind(&DatabaseContextFactory::GetContext, dbContextFactory), rulesetStore, gameHost);

	// 原本要給Working sm做default value，但是現在不用了，所以就直接改成object而不適指標
	//workingSm = new Bindable<WorkingSm*>();

	return 0;
}

MeteoGameBase::MeteoGameBase(): RegisterType("MeteoGameBase")
{
}

Bindable<WorkingSm*>* MeteoGameBase::GetWorkingSm()
{
	return &workingSm;
}