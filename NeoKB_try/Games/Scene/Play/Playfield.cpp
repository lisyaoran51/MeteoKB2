#include "Playfield.h"

#include "../../Scheduler/Event/Effect/EffectMapper.h"
#include "../../../Util/Log.h"
#include "../../Scheduler/Event/IoEvents/IoEventProcessor.h"
#include "../../Scheduler/Event/InstrumentEvents/InstrumentEventProcessor.h"
#include "../../Scheduler/Event/PlayfieldEvents/PlayfieldEventProcessor.h"
#include "../../Scheduler/Event/TimeEvents/TimeEventProcessor.h"
#include <functional>


using namespace Games::Scenes::Play;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers;
using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::IoEvents;
using namespace Games::Schedulers::Events::InstrumentEvents;
using namespace Util;
using namespace std;
using namespace Games::Schedulers::Events::PlayfieldEvents;
using namespace Games::Schedulers::Events::TimeEvents;


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

	isPresent = true;

	//scheduler = new Scheduler();
	eventProcessorMaster = createEventProcessorMaster();
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
	isInputable = true;
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
	LOG(LogLevel::Finer) << "Playfield::Add(EventProcessor<Event>*) : adding [" << ep->GetEventTypeName() << "] to playfield.";
	eventProcessors.push_back(ep);
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
		//EffectMapperInterface* em = ep->Cast<EffectMapperInterface>();
		//em->RegisterMap(lightMap); //���draw(map, effect)�A�ҥH���Τ��s�@��map
	}
	else if (ep->CanCast<IoEventProcessorInterface>()) {
		string processorType = ep->GetEventTypeName();
		map<string, IoCommunicatorInterface*>::iterator iter = ioCommunicators.find(processorType);

		if (iter != ioCommunicators.end())
		{
			IoCommunicatorInterface* ioCommunicator = ioCommunicators[processorType];
			ep->Cast<IoEventProcessorInterface>()->RegisterIoCommunicator(ioCommunicator);

			LOG(LogLevel::Finer) << "Playfield::Add(EventProcessor<Event>*) : Register [" << ioCommunicators[processorType]->GetTypeName() << "] to processor [" << processorType << "] on [" << ep->GetStartTime() << "].";
		}
		else {
			LOG(LogLevel::Error) << "Playfield::Add(EventProcessor<Event>*) : Register processor [" << processorType << "] failed." << ioCommunicators.size();
			throw runtime_error("Playfield::Add(EventProcessor<Event>*) : Register processor failed.");
		}

	}
	else if (ep->CanCast<InstrumentEventProcessorInterface>()) { // �o��ΨӦ۰ʽ��O
		string processorType = ep->GetEventTypeName();
		map<string, InstrumentControllerInterface*>::iterator iter = instrumentControllers.find(processorType);

		if (iter != instrumentControllers.end())
		{
			InstrumentControllerInterface* instrumentController = instrumentControllers[processorType];
			ep->Cast<InstrumentEventProcessorInterface>()->RegisterInstrumentController(instrumentController);

			LOG(LogLevel::Finer) << "Playfield::Add(EventProcessor<Event>*) : Register [" << instrumentControllers[processorType]->GetTypeName() << "] to processor [" << processorType << "] on [" << ep->GetStartTime() << "].";
		}
		else {
			LOG(LogLevel::Error) << "Playfield::Add(EventProcessor<Event>*) : Register processor [" << processorType << "] failed." << instrumentControllers.size();
			throw runtime_error("Playfield::Add(EventProcessor<Event>*) : Register processor failed.");
		}

	}
	else if (ep->CanCast<PlayfieldEventProcessorInterface>()) { // �o��ΨӦ۰ʲ���
		string processorType = ep->GetEventTypeName();
		map<string, PlayfieldControllerInterface*>::iterator iter = playfieldControllers.find(processorType);

		if (iter != playfieldControllers.end())
		{
			PlayfieldControllerInterface* playfieldController = playfieldControllers[processorType];
			ep->Cast<PlayfieldEventProcessorInterface>()->RegisterPlayfieldController(playfieldController);

			LOG(LogLevel::Finer) << "Playfield::Add(EventProcessor<Event>*) : Register [" << playfieldControllers[processorType]->GetTypeName() << "] to processor [" << processorType << "] on [" << ep->GetStartTime() << "].";
		}
		else {
			LOG(LogLevel::Error) << "Playfield::Add(EventProcessor<Event>*) : Register processor [" << processorType << "] failed." << playfieldControllers.size();
			throw runtime_error("Playfield::Add(EventProcessor<Event>*) : Register processor failed.");
		}
	}
	else if (ep->CanCast<TimeEventProcessorInterface>()) { // �o��ΨӦ۰ʲ���
		string processorType = ep->GetEventTypeName();
		map<string, TimeControllerControllerInterface*>::iterator iter = timeControllerControllers.find(processorType);

		if (iter != timeControllerControllers.end())
		{
			TimeControllerControllerInterface* timeControllerController = timeControllerControllers[processorType];
			ep->Cast<TimeEventProcessorInterface>()->RegisterTimeControllerController(timeControllerController);

			LOG(LogLevel::Finer) << "Playfield::Add(EventProcessor<Event>*) : Register [" << timeControllerControllers[processorType]->GetTypeName() << "] to processor [" << processorType << "] on [" << ep->GetStartTime() << "].";
		}
		else {
			LOG(LogLevel::Error) << "Playfield::Add(EventProcessor<Event>*) : Register processor [" << processorType << "] failed." << timeControllerControllers.size();
			throw runtime_error("Playfield::Add(EventProcessor<Event>*) : Register processor failed.");
		}

	}

	return 0;
}

vector<EventProcessor<Event>*>* Playfield::GetEventProcessors()
{
	return &eventProcessors;
}

int Playfield::AddDynamic(EventProcessor<Event>* ep) {


	// �o��n��Map Algo�[�i�h
	if (ep->CanCast<EffectMapperInterface>()) {

		// �����򤣥�event�ۤv��create? �]���n�h�f�t���P��mapper�A�ҥH�n�ʺA�հt
		string processorType = ep->GetEventTypeName();
		map<string, MapAlgorithmInterface*>::iterator iter = mapAlgorithms.find(processorType);

		if (iter != mapAlgorithms.end())
		{
			MapAlgorithmInterface* mapAlgo = mapAlgorithms[processorType];
			ep->Cast<EffectMapperInterface>()->RegisterMapAlgorithm(mapAlgo);

			LOG(LogLevel::Finer) << "Playfield::AddDynamic() : Register [" << mapAlgorithms[processorType]->GetTypeName() << "] to mapper [" << processorType << "] on [" << ep->GetStartTime() << "].";
		}
		else
			throw runtime_error("Playfield::AddDynamic() : effect map algo not found");

		// �o��n��map�[�i�h
		//EffectMapperInterface* em = ep->Cast<EffectMapperInterface>();
		//em->RegisterMap(lightMap); //���draw(map, effect)�A�ҥH���Τ��s�@��map
	}

	eventProcessorMaster->AddDynamicEventProcessor(ep);

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
