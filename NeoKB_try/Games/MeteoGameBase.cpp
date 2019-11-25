#include "MeteoGameBase.h"

#include <functional>


using namespace Games;
using namespace Games::Rulesets;
using namespace std;


int MeteoGameBase::load()
{

	LOG(LogLevel::Info) << "MeteoGameBase::load() : caching Resources.";

	workingSm.SetValue(nullptr);
	workingSm.AddOnValueChanged(bind(&MeteoGameBase::onWorkingSmValueChanged, this, placeholders::_1), "MeteoGameBase::onWorkingSmValueChanged");
	/* 不知道為什麼lambda式就是不會過
	workingSm.AddOnValueChenged([=](void* wSm){
		
		WorkingSm* w = static_cast<WorkingSm*>(wSm);
		audioManager->GetTrackManager()->AddItem(w->GetTrack());

		return 0;
	}, "WorkingSm::OnValueChanged");
	*/

	LOG(LogLevel::Debug) << "MeteoGameBase::load() : host address = " << gameHost;
	dbContextFactory = new DatabaseContextFactory(gameHost);
	dbContextFactory->Initialize();

	GetDependencies()->Cache<MeteoGameBase>(this, "MeteoGameBase");

	GetDependencies()->Cache<RulesetStore>(rulesetStore = new RulesetStore(bind(&DatabaseContextFactory::GetContext, dbContextFactory)));

	GetDependencies()->Cache<FileStore>(fileStore = new FileStore(bind(&DatabaseContextFactory::GetContext, dbContextFactory), gameHost->GetStorage()));

	GetDependencies()->Cache<KeyBindingStore>(keyBindingStore = new KeyBindingStore(bind(&DatabaseContextFactory::GetContext, dbContextFactory)));

	GetDependencies()->Cache<SmManager>(smManager = new SmManager(gameHost->GetStorage(), bind(&DatabaseContextFactory::GetContext, dbContextFactory), rulesetStore, gameHost));

	// 原本要給Working sm做default value，但是現在不用了，所以就直接改成object而不適指標
	//workingSm = new Bindable<WorkingSm*>();



	return 0;
}

MeteoGameBase::MeteoGameBase(): RegisterType("MeteoGameBase")
{
	registerLoad(bind((int(MeteoGameBase::*)())&MeteoGameBase::load, this));
}

BindablePointer<WorkingSm*>* MeteoGameBase::GetWorkingSm()
{
	return &workingSm;
}

int MeteoGameBase::onWorkingSmValueChanged(void * wSm)
{
	WorkingSm* w = static_cast<WorkingSm*>(wSm);
	audioManager->GetTrackManager()->AddItem(w->GetTrack());

	return 0;
}
