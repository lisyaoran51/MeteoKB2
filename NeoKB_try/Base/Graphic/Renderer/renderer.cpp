#include "renderer.h"
#include "../../../Util/InstanceCreator.h"
#include "../../../Util/MtoObject.h"
#include "../../../Util/Log.h"




using namespace Base::Graphic::Renderers;
using namespace Util;
using namespace Devices;
using namespace std;


map<int, string> Base::Graphic::Renderers::Renderer::renderers;
bool Base::Graphic::Renderers::Renderer::initialized = Base::Graphic::Renderers::Renderer::initialize();

bool Renderer::initialize()
{
	//TODO: 改成讀文件 不要寫在code裡
	for (int i = 0; i < sizeof(hwInfo) / sizeof(hwInfo[0]); i++) {
		renderers[hwInfo[i].HwVersion] = hwInfo[i].Renderer;
	}
	return true;;
}

/*
int Renderer::RegisterHWMapAlgo(HardwareMapAlgo * hwma)
{
	hwMapAlgo = hwma;
	return 0;
}

int Renderer::RegisterLedDriver(LedDriver * ld)
{
	ledDriver = ld;
	return 0;
}
*/

int Renderer::load()
{
	Updater* u = GetCache<Updater>("Updater");
	if (!u)
		throw runtime_error("int EventProcessorMaster::load() : Updater not found in cache.");

	FrameworkConfigManager * f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int EventProcessorMaster::load() : FrameworkConfigManager not found in cache.");

	return load(u, f);
}

int Renderer::load(Updater * u, FrameworkConfigManager* f)
{
	LOG(LogLevel::Info) << "Renderer::load(Updater*, FrameworkConfigManager*) : Register render task to updater";

	u->RegisterTask(bind((int(Renderer::*)(MTO_FLOAT))&Renderer::Elapse, this, placeholders::_1));

	if (!f->Get(FrameworkSetting::FrameRate, &frameRate))
		throw runtime_error("int Renderer::load(Updater*, FrameworkConfigManager*) : FrameRate not found in Setting.");

	frameLength = MTO_FLOAT(1) / MTO_FLOAT(frameRate);
	currentFrameLength = 0;

	return 0;
}

Renderer::Renderer(): RegisterType("Renderer"), ChildAddable()
{
	registerLoad(bind((int(Renderer::*)())&Renderer::load, this));
}

int Renderer::SetHardwareVersion(int hwVersion)
{
	hardwareVersion = hwVersion;
	return 0;
}

int Renderer::RegisterMap(Map * m)
{
	lightMap = m;
	return 0;
}

Renderer * Renderer::GetRenderer(int hwVersion)
{
	initialized;

	string rendererName = renderers[hwVersion];

	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();

	Renderer* renderer = iCreator.CreateInstanceWithT<Renderer>(rendererName);
	
	// TODO: 查看有沒有這個renderer，沒有要丟錯誤

	renderer->SetHardwareVersion(hwVersion);

	LOG(LogLevel::Info) << "Renderer::GetRenderer : Hardware version: [" << hwVersion << "]. Renderer version: [" << rendererName << "].";

	return renderer;
}

int Renderer::Elapse(MTO_FLOAT elapsedTime)
{
	if (elapsedTime == -1) {
		// 遊戲還沒開始時事-1，之後變0代表遊戲開始
		currentTime = 0;
		return 0;
	}


	// 如果時間超過要render圖的周期時間，就開始render圖
	currentFrameLength += elapsedTime;

	LOG(LogLevel::Finest) << "Renderer::Elapse() : elapsed time [" << elapsedTime << "], current frame length [" << currentFrameLength
		<< "], frame length [" << frameLength << "].";

	if (currentFrameLength > frameLength) {
		currentFrameLength -= frameLength;
		Render();
		SendToDriver();
		LOG(LogLevel::Finest) << "Renderer::Elapse() : Rendering done.";
	}

	CleanMap();

	return 0;
}

int Renderer::CleanMap()
{
	lightMap->Reset();
	return 0;
}
