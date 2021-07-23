#include "Playfield.h"

#include "../../Scheduler/Event/Effect/EffectMapper.h"
#include "../../../Util/Log.h"
#include "../../Scheduler/Event/IoEvents/IoEventProcessor.h"
#include "../../Scheduler/Event/InstrumentEvents/InstrumentEventProcessor.h"
#include "../../Scheduler/Event/PlayfieldEvents/PlayfieldEventProcessor.h"
#include "../../Scheduler/Event/TimeEvents/TimeEventProcessor.h"
#include "../../Scheduler/Event/SystemEvents/SystemEventHandler.h"
#include "../../Scheduler/Event/InstrumentEvents/InstrumentEventProcessor.h"
#include <functional>
#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"


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
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Games::Schedulers::Events::InstrumentEvents;
using namespace Games::Output::Bluetooths;


/*
 * 用成員的函數擺進function裡
 * https://zh.cppreference.com/w/cpp/utility/functional/function
 */



int Playfield::load()
{
	LOG(LogLevel::Info) << "Playfield::load() : Start loading game scene";


	LOG(LogLevel::Finer) << "Playfield::load() : getting FrameworkConfigManager";
	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int Playfield::load() : FrameworkConfigManager not found in cache.");

	OutputManager* o = GetCache<OutputManager>("OutputManager");
	if (!f)
		throw runtime_error("int Playfield::load() : OutputManager not found in cache.");

	return load(f, o);
}

int Playfield::load(FrameworkConfigManager* f, OutputManager* o) {

	outputManager = o;

	isPresent = true;

	//scheduler = new Scheduler();
	eventProcessorMaster = createEventProcessorMaster();
	//updater = u;
	dynamicEventGenerator = createDynamicEventGenerator();


	
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
		// 因為只看畫面中央，所以不看其他排
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

	AddChild(dynamicEventGenerator);

}

Playfield::Playfield():RegisterType("Playfield")
{
	isInputable = true;
	// functional裡面的bind不能對overloading問題
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(Playfield::*)())&Playfield::load, this));
}

Playfield::~Playfield()
{
	for (int i = 0; i < eventProcessors.size(); i++)
		delete eventProcessors[i];
	eventProcessors.clear();

	delete bufferMap;
}

int Playfield::LoadOnComplete()
{
	ChangePitchState(MeteoPianoPitchState::None);
	return 0;
}

int Playfield::OnJudgement(HitObject * hitObject, Judgement * judgement)
{
	return 0;
}

int Playfield::Add(EventProcessor<Event> * ep)
{
	LOG(LogLevel::Depricated) << "Playfield::Add(EventProcessor<Event>*) : adding [" << ep->GetEventTypeName() << "] to playfield.";
	eventProcessors.push_back(ep);
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
		//EffectMapperInterface* em = ep->Cast<EffectMapperInterface>();
		//em->RegisterMap(lightMap); //改用draw(map, effect)，所以不用內存一個map
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
	else if (ep->CanCast<InstrumentEventProcessorInterface>()) { // 這邊用來自動踩踏板
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
	else if (ep->CanCast<PlayfieldEventProcessorInterface>()) { // 這邊用來自動移調
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
	else if (ep->CanCast<TimeEventProcessorInterface>()) { // 這邊用來自動移調
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

int Playfield::SetGetEventProcessorFunction(function<EventProcessor<Event>*(Event*)> getEventProcessorFunction)
{
	getEventProcessor = getEventProcessorFunction;

	return 0;
}

int Playfield::SetLeaveGameFunction(function<int()> lGame)
{
	leaveGame = lGame;
	return 0;
}
int Playfield::SetRestartGameFunction(function<int()> rGame)
{
	restartGame = rGame;
	return 0;
}

int Playfield::SetEndGameFunction(function<int()> eGame)
{
	endGame = eGame;
	return 0;
}

vector<EventProcessor<Event>*>* Playfield::GetEventProcessors()
{
	return &eventProcessors;
}

int Playfield::AddDynamic(EventProcessor<Event>* ep) {


	// 這邊要把Map Algo加進去
	if (ep->CanCast<EffectMapperInterface>()) {

		// 為什麼不用event自己來create? 因為要去搭配不同的mapper，所以要動態調配
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

	}
	else if (ep->CanCast<InstrumentEventProcessorInterface>()) {

		// 為什麼不用event自己來create? 因為要去搭配不同的mapper，所以要動態調配
		string processorType = ep->GetEventTypeName();
		map<string, InstrumentControllerInterface*>::iterator iter = instrumentControllers.find(processorType);

		if (iter != instrumentControllers.end())
		{
			InstrumentControllerInterface* instrumentController = instrumentControllers[processorType];
			ep->Cast<InstrumentEventProcessorInterface>()->RegisterInstrumentController(instrumentController);

			LOG(LogLevel::Finer) << "Playfield::AddDynamic() : Register [" << instrumentControllers[processorType]->GetTypeName() << "] to mapper [" << processorType << "] on [" << ep->GetStartTime() << "].";
		}
		else
			throw runtime_error("Playfield::AddDynamic() : effect map algo not found");

	}
	else if (ep->CanCast<PlayfieldEventProcessorInterface>()) { // 這邊用來自動移調
		string processorType = ep->GetEventTypeName();
		map<string, PlayfieldControllerInterface*>::iterator iter = playfieldControllers.find(processorType);

		if (iter != playfieldControllers.end())
		{
			PlayfieldControllerInterface* playfieldController = playfieldControllers[processorType];
			ep->Cast<PlayfieldEventProcessorInterface>()->RegisterPlayfieldController(playfieldController);

			LOG(LogLevel::Finer) << "Playfield::AddDynamic() : Register [" << playfieldControllers[processorType]->GetTypeName() << "] to processor [" << processorType << "] on [" << ep->GetStartTime() << "].";
		}
		else {
			LOG(LogLevel::Error) << "Playfield::AddDynamic() : Register processor [" << processorType << "] failed." << playfieldControllers.size();
			throw runtime_error("Playfield::AddDynamic() : Register processor failed.");
		}
	}
	else if (ep->CanCast<SystemEventHandlerInterface>()) {
		// 為什麼不用event自己來create? 因為要去搭配不同的mapper，所以要動態調配
		string processorType = ep->GetEventTypeName();
		map<string, SystemController*>::iterator iter = systemControllers.find(processorType);

		if (iter != systemControllers.end())
		{
			SystemController* systemController = systemControllers[processorType];
			ep->Cast<SystemEventHandlerInterface>()->RegisterSystemController(systemController);

			LOG(LogLevel::Finer) << "Playfield::AddDynamic() : Register [" << systemControllers[processorType]->GetTypeName() << "] to event handler [" << processorType << "] on [" << ep->GetStartTime() << "].";
		}
		else
			throw runtime_error("Playfield::AddDynamic() : event handler not found");
	}
	else {
		throw runtime_error("Playfield::AddDynamic() : no matched inteface. event handler not found");
	}

	eventProcessorMaster->AddDynamicEventProcessor(ep);

	return 0;
}

int Playfield::AddDynamic(Event * e)
{
	return AddDynamic(getEventProcessor(e));
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

DynamicEventGenerator * Playfield::GetDynamicEventGenerator()
{
	return dynamicEventGenerator;
}

int Playfield::SetIsGameControllingPitchState(bool value)
{
	isGameControllingPitchState = value;
	return 0;
}

int Playfield::ChangePitchState(MeteoPianoPitchState s)
{

	if (s == MeteoPianoPitchState::Lowered) {
		pitchState = MeteoPianoPitchState::Lowered;
		eventProcessorMaster->ChangePitchState(MeteoPianoPitchState::Lowered);
	}
	else if (s == MeteoPianoPitchState::None) {
		pitchState = MeteoPianoPitchState::None;
		eventProcessorMaster->ChangePitchState(MeteoPianoPitchState::None);
	}
	else if (s == MeteoPianoPitchState::Raised) {
		pitchState = MeteoPianoPitchState::Raised;
		eventProcessorMaster->ChangePitchState(MeteoPianoPitchState::Raised);
	}
	return 0;
}

MeteoPianoPitchState Playfield::GetMeteoPianoPitchState()
{
	return pitchState;
}

int Playfield::GetXPositionFromPitch(Pitch p)
{
	int xPosition = (int)p - int(startPitch);

	switch (pitchState) {
	case MeteoPianoPitchState::None:
		xPosition -= 12;
		break;
	case MeteoPianoPitchState::Raised:
		xPosition -= 24;
		break;
	}

	if (xPosition < 0 || xPosition >= pitchCount)
		return -1;

	return xPosition;
}
