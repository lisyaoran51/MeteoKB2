#include "Base.h"


#include "IO/Stores/NamespacedResourceStore.h"
#include "IO/Stores/DynamicPathResourceStore.h"
#include "Threading/ThreadMaster.h"
#include "Threading/SimpleThread.h"

using namespace Framework;
using namespace Framework::Allocation;
using namespace Framework::IO;
using namespace Framework::Audio;
using namespace Framework::Hosts;
using namespace Framework::IO::Stores;
using namespace Framework::Threading;



int Base::load()
{
	LOG(LogLevel::Info) << "Base::load() : caching audio manager and main resource.";

	resources = new CompositeResourceStore<char*>();
	resources->AddStore(new DynamicPathResourceStore(GetResourcePath()));


	auto tracks = new NamespacedResourceStore<char*>(resources, "Tracks");
	auto samples = new NamespacedResourceStore<char*>(resources, "Samples");

	audioManager = new AudioManager(tracks, samples);

	Cache<AudioManager>(audioManager);

	outputManager = new OutputManager(gameHost->GetMainInterface());

	setupOutputManager();

	Cache<OutputManager>(outputManager);

	setupGameStatusActions();

	return 0;
}

Base::Base() : RegisterType("Base")
{
	registerLoad(bind((int(Base::*)())&Base::load, this));
}

int Base::SetHost(Host * host)
{
	LOG(LogLevel::Depricated) << "Base::SetHost() : host address = " << host;

	gameHost = host;
	return 0;
}

string Base::GetResourcePath()
{
	return string("/home/pi/SystemData/Resources");
}

int Base::setupOutputManager()
{
	if (gameHost)
		gameHost->SetupOutputManager(outputManager);
	else
		throw runtime_error("Base::setupOutputManager() : no host.");

	return 0;
}

int Base::setupGameStatusActions()
{
	/* 演奏模式，只要輸入和音樂快就好 */
	ThreadMaster::GetInstance().AddGameStatusThreadAction((int)GameStatus::Perform, this, [=]() {

		if (ThreadMaster::GetInstance().GetThread("InputThread"))
			ThreadMaster::GetInstance().GetThread("InputThread")->SetMaxUpdateHz(500);

		if (ThreadMaster::GetInstance().GetThread("DrawThread"))
			ThreadMaster::GetInstance().GetThread("DrawThread")->SetMaxUpdateHz(1);

		if (ThreadMaster::GetInstance().GetThread("UpdateThread"))
			ThreadMaster::GetInstance().GetThread("UpdateThread")->SetMaxUpdateHz(10);

		if (ThreadMaster::GetInstance().GetThread("AudioThread"))
			ThreadMaster::GetInstance().GetThread("AudioThread")->SetMaxUpdateHz(500);

		if (ThreadMaster::GetInstance().GetSimpleThread("MeteoMcu"))
			ThreadMaster::GetInstance().GetSimpleThread("MeteoMcu")->SetSleepTimeInMilliSecond(0);

		if (ThreadMaster::GetInstance().GetSimpleThread("ForegroundBleAccess"))
			ThreadMaster::GetInstance().GetSimpleThread("ForegroundBleAccess")->SetSleepTimeInMilliSecond(500);

		return 0;
	}, "Base::Lambda_SetupPerformGameStatusThreadAction");

	/* 遊戲模式，只要輸入和音樂、影像快 */
	ThreadMaster::GetInstance().AddGameStatusThreadAction((int)GameStatus::Game, this, [=]() {

		if (ThreadMaster::GetInstance().GetThread("InputThread"))
			ThreadMaster::GetInstance().GetThread("InputThread")->SetMaxUpdateHz(500);

		if (ThreadMaster::GetInstance().GetThread("DrawThread"))
			ThreadMaster::GetInstance().GetThread("DrawThread")->SetMaxUpdateHz(30);

		if (ThreadMaster::GetInstance().GetThread("UpdateThread"))
			ThreadMaster::GetInstance().GetThread("UpdateThread")->SetMaxUpdateHz(50);

		if (ThreadMaster::GetInstance().GetThread("AudioThread"))
			ThreadMaster::GetInstance().GetThread("AudioThread")->SetMaxUpdateHz(500);

		if (ThreadMaster::GetInstance().GetSimpleThread("MeteoMcu"))
			ThreadMaster::GetInstance().GetSimpleThread("MeteoMcu")->SetSleepTimeInMilliSecond(0);

		if (ThreadMaster::GetInstance().GetSimpleThread("ForegroundBleAccess"))
			ThreadMaster::GetInstance().GetSimpleThread("ForegroundBleAccess")->SetSleepTimeInMilliSecond(500);


		return 0;
	}, "Base::Lambda_SetupGameGameStatusThreadAction");

	/* 下載模式，只要輸入快 */
	ThreadMaster::GetInstance().AddGameStatusThreadAction((int)GameStatus::Download, this, [=]() {

		if (ThreadMaster::GetInstance().GetThread("InputThread"))
			ThreadMaster::GetInstance().GetThread("InputThread")->SetMaxUpdateHz(50);

		if (ThreadMaster::GetInstance().GetThread("DrawThread"))
			ThreadMaster::GetInstance().GetThread("DrawThread")->SetMaxUpdateHz(1);

		if (ThreadMaster::GetInstance().GetThread("UpdateThread"))
			ThreadMaster::GetInstance().GetThread("UpdateThread")->SetMaxUpdateHz(5);

		if (ThreadMaster::GetInstance().GetThread("AudioThread"))
			ThreadMaster::GetInstance().GetThread("AudioThread")->SetMaxUpdateHz(10);

		if (ThreadMaster::GetInstance().GetSimpleThread("MeteoMcu"))
			ThreadMaster::GetInstance().GetSimpleThread("MeteoMcu")->SetSleepTimeInMilliSecond(500);

		if (ThreadMaster::GetInstance().GetSimpleThread("ForegroundBleAccess"))
			ThreadMaster::GetInstance().GetSimpleThread("ForegroundBleAccess")->SetSleepTimeInMilliSecond(100);


		return 0;
	}, "Base::Lambda_SetupDownloadGameStatusThreadAction");

	ThreadMaster::GetInstance().AddGameStatusThreadAction((int)GameStatus::Sleep, this, [=]() {

		if (ThreadMaster::GetInstance().GetThread("InputThread"))
			ThreadMaster::GetInstance().GetThread("InputThread")->SetMaxUpdateHz(10);

		if (ThreadMaster::GetInstance().GetThread("DrawThread"))
			ThreadMaster::GetInstance().GetThread("DrawThread")->SetMaxUpdateHz(1);

		if (ThreadMaster::GetInstance().GetThread("UpdateThread"))
			ThreadMaster::GetInstance().GetThread("UpdateThread")->SetMaxUpdateHz(10);

		if (ThreadMaster::GetInstance().GetThread("AudioThread"))
			ThreadMaster::GetInstance().GetThread("AudioThread")->SetMaxUpdateHz(1);

		if (ThreadMaster::GetInstance().GetSimpleThread("MeteoMcu"))
			ThreadMaster::GetInstance().GetSimpleThread("MeteoMcu")->SetSleepTimeInMilliSecond(10);

		if (ThreadMaster::GetInstance().GetSimpleThread("ForegroundBleAccess"))
			ThreadMaster::GetInstance().GetSimpleThread("ForegroundBleAccess")->SetSleepTimeInMilliSecond(500);

		return 0;
	}, "Base::Lambda_SetupSleepGameStatusThreadAction");
	return 0;
}

