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


	LOG(LogLevel::Depricated) << [](int width, int height, Map* m) {
		m->Add(width, height, 30);
		m->Add(0, 0, 60);
		m->Add(5, 5, 90);
		m->Add(width + 5, height + 5, 120);
		LOG(LogLevel::Finest) << "Playfield::load() : testing map add.";
		// �]���u�ݵe�������A�ҥH���ݨ�L��
		for (int i = 0; i < width * 2; i++) {
			string s;
			for (int j = 0; j < height * 2; j++) {
				s += to_string(m->Get(i, j));
				s += " ";
			}
			LOG(LogLevel::Finest) << "| " << s << "|";
		}
		return 0;
	}(width, height, bufferMap);

	// �ھڹC���j�p�A�ؤ@��map
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

	// �o�@�B�O���L�̥h��updater
	LOG(LogLevel::Fine) << "Playfield::load() : Adding event proessor master ...";
	AddChild(eventProcessorMaster);

}

Playfield::Playfield():RegisterType("Playfield")
{
	// functional�̭���bind�����overloading���D
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(Playfield::*)())&Playfield::load, this));
}

int Playfield::OnJudgement(HitObject * hitObject, Judgement * judgement)
{
	return 0;
}

int Playfield::Add(EventProcessor<Event> * ep)
{
	//scheduler->Add(ep);
	eventProcessorMaster->AddStaticEventProcessor(ep);

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
		//em->RegisterMap(lightMap); //���draw(map, effect)�A�ҥH���Τ��s�@��map
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
