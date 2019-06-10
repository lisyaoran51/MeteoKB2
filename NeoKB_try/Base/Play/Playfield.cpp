#include "Playfield.h"
#include "../Scheduler/Event/Effect/EffectMapper.h"
#include "../../Util/Log.h"
#include <functional>


using namespace Base::Play;
using namespace Base::Schedulers::Events;
using namespace Base::Schedulers;
using namespace Base::Schedulers::Events::Effects;
using namespace Util;
using namespace std;


/*
 * 用成員的函數擺進function裡
 * https://zh.cppreference.com/w/cpp/utility/functional/function
 */



int Playfield::load()
{
	LOG(LogLevel::Info) << "Playfield::load() : Start loading game scene";


	//LOG(LogLevel::Finer) << "Playfield::load() : getting scheduler";
	//Scheduler* s = GetCache<Scheduler>("Scheduler");
	//if (!s)
	//	throw runtime_error("int Playfield::load() : Scheduler not found in cache.");

	//LOG(LogLevel::Finer) << "Playfield::load() : getting EventProcessorMaster";
	//EventProcessorMaster* e = GetCache<EventProcessorMaster>("EventProcessorMaster");
	//if (!e)
	//	throw runtime_error("int Playfield::load() : EventProcessorMaster not found in cache.");

	LOG(LogLevel::Finer) << "Playfield::load() : getting FrameworkConfigManager";
	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int Playfield::load() : FrameworkConfigManager not found in cache.");

	LOG(LogLevel::Finer) << "Playfield::load() : getting Updater";
	Updater* u = GetCache<Updater>("Updater");
	if (!u)
		throw runtime_error("int Playfield::load() : Updater not found in cache.");

	return load(f, u);
}

int Playfield::load(FrameworkConfigManager* f, Updater* u) {

	

	scheduler = new Scheduler();
	eventProcessorMaster = new EventProcessorMaster();
	updater = u;


	int hwVersion;
	if (f->Get(FrameworkSetting::HardwareVersion, &hwVersion)) {
		LOG(LogLevel::Fine) << "Playfield::load() : getting renderer of version [" << hwVersion << "].";
		renderer = Renderer::GetRenderer(hwVersion);
	}
	else
		throw runtime_error("int Playfield::load() : HardwareVersion not found in Setting.");
	

	// 根據遊戲大小，建一個map
	if (f->Get(FrameworkSetting::Width, &width) &&
		f->Get(FrameworkSetting::Height, &height))
	{
		LOG(LogLevel::Fine) << "Playfield::load() : building map of size [" << width << "] * [" << height << "].";
		lightMap = new Map(width, height);
		bufferMap = new Map(width * 2, height * 2);
	}
	else
		throw runtime_error("int Playfield::load() : Width and Height not found in Setting.");
	
	LOG(LogLevel::Fine) << "Playfield::load() : Registering map ...";
	eventProcessorMaster->RegisterMap(lightMap);
	renderer->RegisterMap(lightMap);

	LOG(LogLevel::Fine) << "Playfield::load() : Registering event process master to scheduler ...";
	scheduler->RegisterHandler(bind(&EventProcessorMaster::ReceiveEventProcessor, eventProcessorMaster, placeholders::_1));

	// 這一步是讓他們去抓updater
	LOG(LogLevel::Fine) << "Playfield::load() : Adding scheduler, event proessor master and ernderer ...";
	AddChild(scheduler);
	AddChild(eventProcessorMaster);
	AddChild(renderer);

	// 最後也要把playfield加進updater裡，但是應該不能寫在這，要寫在繼承的class上
}

Playfield::Playfield():RegisterType("Playfield")
{
	// functional裡面的bind不能對overloading問題
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(Playfield::*)())&Playfield::load, this));
}

int Playfield::Add(EventProcessor<Event> * ep)
{
	scheduler->Add(ep);

	// 這邊要把Map Algo加進去
	if (ep->CanCast<EffectMapperInterface>()) {

		// 為什麼不用event自己來create? 因為要去搭配不同的mapper，所以要動態調配
		string processorType = ep->GetEventTypeName();
		map<string, MapAlgorithmInterface*>::iterator iter = mapAlgorithms.find(processorType);

		if (iter != mapAlgorithms.end())
		{
			MapAlgorithmInterface* mapAlgo = mapAlgorithms[processorType];
			ep->Cast<EffectMapperInterface>()->RegisterMapAlgorithm(mapAlgo);

			LOG(LogLevel::Finer) << "Playfield::Add(EventProcessor<Event>*) : Register [" << mapAlgorithms[processorType]->GetTypeName() << "] to mapper [" << processorType << "] on [" << ep->GetStartTime() << "].";
		}

		// 這邊要把map加進去
		EffectMapperInterface* em = ep->Cast<EffectMapperInterface>();
		em->RegisterMap(lightMap);
	}

	return 0;
}

int Playfield::GetWidth()
{
	return width;
}

int Playfield::GetHeight()
{
	return height;
}
