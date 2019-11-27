#include "Playfield.h"
#include "../../Scheduler/Event/Effect/EffectMapper.h"
#include "../../Util/Log.h"
#include <functional>


using namespace Games::Scenes::Play;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers;
using namespace Games::Schedulers::Events::Effects;
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

	return load(f);
}

int Playfield::load(FrameworkConfigManager* f) {

	

	//scheduler = new Scheduler();
	eventProcessorMaster = new EventProcessorMaster();
	//updater = u;


	int hwVersion;
	if (f->Get(FrameworkSetting::HardwareVersion, &hwVersion)) {
		LOG(LogLevel::Fine) << "Playfield::load() : getting renderer of version [" << hwVersion << "].";
		//renderer = Renderer::GetRenderer(hwVersion);
	}
	else
		throw runtime_error("int Playfield::load() : HardwareVersion not found in Setting.");
	
	if (f->Get(FrameworkSetting::Width, &width) &&
		f->Get(FrameworkSetting::Height, &height)) {
		bufferMap = new Map(width * 2, height * 2);
		LOG(LogLevel::Fine) << "Playfield::load() : build buffer map [" << width << "*2] * [" << height << "*2].";
	}
	else
		throw runtime_error("int Playfield::load() : Width and Height not found in Setting.");

	// 根據遊戲大小，建一個map
	//if (f->Get(FrameworkSetting::Width, &width) &&
	//	f->Get(FrameworkSetting::Height, &height))
	//{
	//	LOG(LogLevel::Fine) << "Playfield::load() : building map of size [" << width << "] * [" << height << "].";
	//	lightMap = new Map(width, height);
	//	bufferMap = new Map(width * 2, height * 2);
	//}
	//else
	//	throw runtime_error("int Playfield::load() : Width and Height not found in Setting.");
	
	//LOG(LogLevel::Fine) << "Playfield::load() : Registering map ...";
	//eventProcessorMaster->RegisterMap(lightMap);
	//renderer->RegisterMap(lightMap);

	// 這一步是讓他們去抓updater
	LOG(LogLevel::Fine) << "Playfield::load() : Adding event proessor master ...";
	AddChild(eventProcessorMaster);

}

Playfield::Playfield():RegisterType("Playfield")
{
	// functional裡面的bind不能對overloading問題
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(Playfield::*)())&Playfield::load, this));
}

int Playfield::Add(EventProcessor<Event> * ep)
{
	//scheduler->Add(ep);
	eventProcessorMaster->AddStaticEventProcessor(ep);

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
		//em->RegisterMap(lightMap); //改用draw(map, effect)，所以不用內存一個map
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

EventProcessorMaster * Playfield::GetEventProcessorMaster()
{
	return eventProcessorMaster;
}
