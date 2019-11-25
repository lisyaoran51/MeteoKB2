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
	/* �����D������lambda���N�O���|�L
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

	// �쥻�n��Working sm��default value�A���O�{�b���ΤF�A�ҥH�N�����令object�Ӥ��A����
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
