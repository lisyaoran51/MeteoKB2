#include "Playfield.h"
#include "../../Scheduler/Event/Effect/EffectMapper.h"
#include "../../Util/Log.h"
#include <functional>


using namespace Base::Scenes::Play;
using namespace Base::Schedulers::Events;
using namespace Base::Schedulers;
using namespace Base::Schedulers::Events::Effects;
using namespace Util;
using namespace std;


/*
 * �Φ���������\�ifunction��
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
	

	// �ھڹC���j�p�A�ؤ@��map
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

	// �o�@�B�O���L�̥h��updater
	LOG(LogLevel::Fine) << "Playfield::load() : Adding scheduler, event proessor master and ernderer ...";
	AddChild(scheduler);
	AddChild(eventProcessorMaster);
	AddChild(renderer);

	// �̫�]�n��playfield�[�iupdater�̡A���O���Ӥ���g�b�o�A�n�g�b�~�Ӫ�class�W
}

Playfield::Playfield():RegisterType("Playfield")
{
	// functional�̭���bind�����overloading���D
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(Playfield::*)())&Playfield::load, this));
}

int Playfield::Add(EventProcessor<Event> * ep)
{
	scheduler->Add(ep);

	// �o��n��Map Algo�[�i�h
	if (ep->CanCast<EffectMapperInterface>()) {

		// �����򤣥�event�ۤv��create? �]���n�h�f�t���P��mapper�A�ҥH�n�ʺA�հt
		string processorType = ep->GetEventTypeName();
		map<string, MapAlgorithmInterface*>::iterator iter = mapAlgorithms.find(processorType);

		if (iter != mapAlgorithms.end())
		{
			MapAlgorithmInterface* mapAlgo = mapAlgorithms[processorType];
			ep->Cast<EffectMapperInterface>()->RegisterMapAlgorithm(mapAlgo);

			LOG(LogLevel::Finer) << "Playfield::Add(EventProcessor<Event>*) : Register [" << mapAlgorithms[processorType]->GetTypeName() << "] to mapper [" << processorType << "] on [" << ep->GetStartTime() << "].";
		}

		// �o��n��map�[�i�h
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
