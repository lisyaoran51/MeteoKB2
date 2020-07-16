#include "MeteoGameBase.h"


#include "../Framework/Database/MemoryBasedDatabaseContextFactory.h"
#include "../Framework/Database/ReadonlyCsvDatabaseContextFactory.h"
#include <functional>


using namespace Games;
using namespace Games::Rulesets;
using namespace std;


int MeteoGameBase::load()
{

	LOG(LogLevel::Info) << "MeteoGameBase::load() : caching Resources. working sm = [" << &workingSm << "].";

	workingSm.SetValue(nullptr);
	//workingSm.AddOnValueChanged(this, bind(&MeteoGameBase::onWorkingSmValueChanged, this, placeholders::_1), "MeteoGameBase::onWorkingSmValueChanged");
	// /*不知道為什麼lambda式就是不會過
	workingSm.AddOnValueChanged(this, [=](){
		
		LOG(LogLevel::Debug) << "MeteoGameBase::load() : getting new sm's track from audio manager [" << audioManager << "].";

		WorkingSm* w = workingSm.GetValue();
		Track* t = w->GetTrack();
		LOG(LogLevel::Debug) << "MeteoGameBase::load() : track manager is [" << audioManager->GetTrackManager() << "], track is [" << t << "].";
		if(t != nullptr)
			audioManager->GetTrackManager()->AddItem(t);

		return 0;
	}, "WorkingSm::OnValueChanged");
	// */

	LOG(LogLevel::Debug) << "MeteoGameBase::load() : host address = " << gameHost;
	dbContextFactory = new DatabaseContextFactory(gameHost);
	dbContextFactory->Initialize();

	readonlyDbContextFactory = new ReadonlyCsvDatabaseContextFactory(gameHost);
	readonlyDbContextFactory->Initialize();

	volitileDbContextFactory = new MemoryBasedDatabaseContextFactory();
	volitileDbContextFactory->Initialize();

	GetDependencies()->Cache<MeteoGameBase>(this, "MeteoGameBase");

	GetDependencies()->Cache<MeteoConfigManager>(localConfig);

	GetDependencies()->Cache<RulesetStore>(rulesetStore = new RulesetStore(bind(&DatabaseContextFactory::GetContext, readonlyDbContextFactory)));

	GetDependencies()->Cache<FileStore>(fileStore = new FileStore(bind(&DatabaseContextFactory::GetContext, volitileDbContextFactory), gameHost->GetStorage()));

	GetDependencies()->Cache<KeyBindingStore>(keyBindingStore = new KeyBindingStore(bind(&DatabaseContextFactory::GetContext, volitileDbContextFactory)));

	GetDependencies()->Cache<SmManager>(smManager = new SmManager(gameHost->GetStorage(), bind(&DatabaseContextFactory::GetContext, volitileDbContextFactory), rulesetStore, gameHost));

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

int MeteoGameBase::SetHost(GameHost * host)
{
	localConfig = new MeteoConfigManager();
	localConfig->Initialize();
	return Base::SetHost(host);
}

int MeteoGameBase::update()
{
	LOG(LogLevel::Depricated) << "MeteoGameBase::update() : print audio components." << [](AudioManager* a) {
		a->_DebugPrintComponents("|");
		return 0;
	}(audioManager);

	return 0;
}

int MeteoGameBase::onWorkingSmValueChanged(void * wSm)
{
	WorkingSm* w = static_cast<WorkingSm*>(wSm);
	audioManager->GetTrackManager()->AddItem(w->GetTrack());

	return 0;
}
