#include "EventProcessorMaster.h"

#include "../../../Util/Log.h"
#include "Effect/EffectMapper.h"
#include <iomanip>
#include "HitObject.h"
#include "IoEvents/IoEventProcessor.h"
#include "InstrumentEvents/InstrumentEventProcessor.h"
#include "PlayfieldEvents/PlayfieldEventProcessor.h"
#include "TimeEvents/TimeEventProcessor.h"
#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Util/DataStructure/SecondPeriodList.h"


using namespace Games::Schedulers::Events;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::IoEvents;
using namespace std;
using namespace Games::Schedulers::Events::InstrumentEvents;
using namespace Games::Schedulers::Events::PlayfieldEvents;
using namespace Games::Schedulers::Events::TimeEvents;
using namespace Games::Output::Bluetooths;
using namespace Util::DataStructure;




int EventProcessorMaster::load()
{
	LOG(LogLevel::Finer) << "EventProcessorMaster::load() : getting FrameworkConfigManager";
	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int EventProcessorMaster::load() : FrameworkConfigManager not found in cache.");

	EventProcessorFilter* e = GetCache<EventProcessorFilter>("EventProcessorFilter");
	if (!e)
		throw runtime_error("int EventProcessorMaster::load() : EventProcessorFilter not found in cache.");

	OutputManager* o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("int EventProcessorMaster::load() : OutputManager not found in cache.");

	TimeController* t = GetCache<TimeController>("TimeController");
	if (!t)
		throw runtime_error("int EventProcessorMaster::load() : TimeController not found in cache.");

	return load(f, e, o, t);
}


int EventProcessorMaster::load(FrameworkConfigManager * f, EventProcessorFilter * e, OutputManager* o, TimeController* t)
{
	eventProcessorFilter = e;
	outputManager = o;
	timeController = t;

	isPresent = true;
	// TODO: 去framework config manager拿period map要切成多寬一段 ex:每5秒一段
	// 目前先暫訂5秒一段
	int periodMapInterval = 3;
	if (!f->Get(FrameworkSetting::PeriodMapInterval, &periodMapInterval))
		periodMapInterval = 3;

	visibleTimeRange = periodMapInterval;

	// TODO: visible time length也應該要去framework config manager拿
	//eventProcessorPeriods = new PeriodMap<EventProcessor<Event>*>(0, periodMapInterval, [=](EventProcessor<Event>* ep)->pair<float, float> {
	//
	//	float startTime = ep->GetStartTime() - visibleTimeRange;
	//	float endTime = ep->GetStartTime() + ep->GetLifeTime() + visibleTimeRange;
	//
	//	return make_pair(startTime, endTime);
	//});

	/* 改為使用更快的second period map */
	//eventProcessorPeriods = new SecondPeriodMap<EventProcessor<Event>*>([=](EventProcessor<Event>* ep)->pair<float, float> {
	//
	//	float startTime = ep->GetStartTime();// -visibleTimeRange;	// visible time range擺在GetItemsContainPeriods那邊用就好，這邊就不要加了
	//	float endTime = ep->GetStartTime() + ep->GetLifeTime();// +visibleTimeRange;
	//
	//	return make_pair(startTime, endTime);
	//});

	/* 再改為使用更快的second period list */
	eventProcessorPeriods = new SecondPeriodList<EventProcessor<Event>*>([=](EventProcessor<Event>* ep)->pair<float, float> {
	
		float startTime = ep->GetStartTime();// -visibleTimeRange;	// visible time range擺在GetItemsContainPeriods那邊用就好，這邊就不要加了
		float endTime = ep->GetStartTime() + ep->GetLifeTime();// +visibleTimeRange;
	
		return make_pair(startTime, endTime);
	});

	int width, height;

	if (f->Get(FrameworkSetting::Width, &width) &&
		f->Get(FrameworkSetting::Height, &height)) {
		LOG(LogLevel::Fine) << "EventProcessorMaster::load() : intialize drawable size [" << width << "] * [" << height << "].";
		Initialize(width, height);
	}
	else
		throw runtime_error("int EventProcessorMaster::load() : Width and Height not found in Setting.");

	timeController->AddOnPause(this, [=]() {
		isGamePaused = true;
		return 0;
	}, "EventProcessorMaster::Lambda_HandleOnPause");

	timeController->AddOnPauseEnd(this, [=]() {
		isGamePaused = false;
		return 0;
	}, "EventProcessorMaster::Lambda_HandleOnPauseEnd");



	return 0;
}


EventProcessorMaster::EventProcessorMaster(): RegisterType("EventProcessorMaster"), Container()
{
	isInputable = true;
	registerLoad(bind((int(EventProcessorMaster::*)())&EventProcessorMaster::load, this));
}

EventProcessorMaster::~EventProcessorMaster()
{
	delete eventProcessorPeriods; //??
}

int EventProcessorMaster::AddStaticEventProcessor(EventProcessor<Event>* sEventProcessor)
{
	LOG(LogLevel::Depricated) << "EventProcessorMaster::AddStaticEventProcessor : add processor [" << sEventProcessor->GetEvent()->GetTypeName() << "] at [" << sEventProcessor->GetStartTime() << "," << sEventProcessor->GetLifeTime() << "] from event [" << sEventProcessor->GetEvent()->GetStartTime() << "].";


	sEventProcessor->Attach(this);
	staticEventProcessors.push_back(sEventProcessor);

	if(eventProcessorPeriods == nullptr)
		throw runtime_error("int EventProcessorMaster::AddStaticEventProcessor(EventProcessor*) : eventProcessorPeriods should've been loaded.");

	eventProcessorPeriods->InsertItem(sEventProcessor);

	return 0;
}

int EventProcessorMaster::AddDynamicEventProcessor(EventProcessor<Event>* dEventProcessor)
{
	
	dEventProcessor->Attach(this);

	/* 每次要用dynamic processors時，就要鎖起來 */
	lock_guard<mutex> guard(processorsMutex);

	dynamicEventProcessors.push_back(dEventProcessor);
	LOG(LogLevel::Depricated) << "EventProcessorMaster::AddDynamicEventProcessor : add explosion [" << dEventProcessor << "], dynamic size = [" << dynamicEventProcessors.size() << "].";
	return 0;
}

int EventProcessorMaster::Clean()
{
	//eventProcessors->clear();
	return 0;
}

PeriodMap<EventProcessor<Event>*>* EventProcessorMaster::GetEventProcessorPeriods()
{
	return eventProcessorPeriods;;
}

int EventProcessorMaster::ChangePitchState(MeteoPianoPitchState pState)
{
	pitchState = pState;
	return 0;
}


int EventProcessorMaster::processEvent(MTO_FLOAT elapsedTime)
{
	vector<EventProcessor<Event>*> eventProcessors;

	double currentTime = 0;
	try {
		currentTime = GetClock()->GetCurrentTime();
	}
	catch (exception& e) {
		LOG(LogLevel::Warning) << "EventProcessorMaster::processEvent : clock is not started [" << e.what() << "].";
		return -1;
		//abort();
	}

	/*
	 * 正轉的狀態(快轉的狀態不執行，避免所有事件突然全部跳出來)
	 */
	if (elapsedTime > 0) {

		eventProcessorPeriods->GetItemsContainPeriods(make_pair<float, float>(currentTime - visibleTimeRange, currentTime + visibleTimeRange * 2), &eventProcessors);	// visible time range * 2是因為好像讀不到miss

		LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent() : check event processors is mapper." << [=]() {
			for (int i = 0; i < eventProcessors.size(); i++) {
				if (dynamic_cast<EffectMapperInterface*>(eventProcessors[i])) {
					LOG(LogLevel::Debug) << "EventProcessorMaster::processEvent() :  effect mapper [" << eventProcessors[i]->GetStartTime() << "].";
				}
			}
			return 0;
		}();
		
		LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent() : get processors size[" << eventProcessors.size() << "].";

		// 移到遊戲最一開始就把所有event processor都先filter掉，這邊就只處理即時的filter
		LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent() : filter event processors.";
		if (eventProcessorFilter->GetIsGameTimeFiltering()) {
			
			filteredTempStaticEventProcessors.clear();
			for (int i = 0; i < eventProcessors.size(); i++) {
				if (eventProcessorFilter->GameTimeFilter(eventProcessors[i])) {
					filteredTempStaticEventProcessors.push_back(eventProcessors[i]);
				}
			}

		}
		else {
			filteredTempStaticEventProcessors.assign(eventProcessors.begin(), eventProcessors.end());
		}


		for (int i = 0; i < filteredTempStaticEventProcessors.size(); i++) {

			LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : this processor is for [" << filteredTempStaticEventProcessors[i]->GetEvent()->GetTypeName() << "] [" << filteredTempStaticEventProcessors[i]->GetStartTime() << "]. " << currentTime;

			if (filteredTempStaticEventProcessors[i]->GetStartTime() >= currentTime)
				continue;

			if (timeController->GetTimeControllerState() != TimeControllerState::FastForward) {
				// TODO: 直接改成 eventProcessor.Process()就好，下面可以全部刪掉
				if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Io) {
					IoEventProcessorInterface* ioEventProcessors = dynamic_cast<IoEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
					if (ioEventProcessors) {
						if (ioEventProcessors->GetStartTime() < currentTime && ioEventProcessors->GetIsTransferable()) {
							LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : found io event processor [" << ioEventProcessors->GetStartTime() << "].";
							ioEventProcessors->ProcessIo();
						}
					}
					continue;
				}

				if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Instrument) {
					InstrumentEventProcessorInterface* instrumentEventProcessor = dynamic_cast<InstrumentEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
					if (instrumentEventProcessor) {
						if (instrumentEventProcessor->GetStartTime() < currentTime && instrumentEventProcessor->GetIsTransferable()) {
							LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : found instrument event processor [" << instrumentEventProcessor->GetStartTime() << "].";
							instrumentEventProcessor->ControlInstrument();
						}
					}
					continue;
				}

				if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Time) {
					TimeEventProcessorInterface* timeEventProcessor = dynamic_cast<TimeEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
					if (timeEventProcessor) {
						LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : found time event processor [" << timeEventProcessor->GetStartTime() << "]. now time " << currentTime;
						if (timeEventProcessor->GetStartTime() < currentTime && timeEventProcessor->GetIsProcessable()) {
							LOG(LogLevel::Debug) << "EventProcessorMaster::processEvent : found time event processor [" << timeEventProcessor->GetStartTime() << "]. " << currentTime;
							timeEventProcessor->ControllTimeController();
							break;
						}
					}
					continue;
				}
			}
			else {
				/* 如果是在快轉狀態，就把這個event直接轉成已執行 */
				if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Io) {
					IoEventProcessorInterface* ioEventProcessors = dynamic_cast<IoEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
					if (ioEventProcessors) {
						if (ioEventProcessors->GetStartTime() < currentTime && ioEventProcessors->GetIsTransferable()) {
							ioEventProcessors->SetIsTransfered();
						}
					}
					continue;
				}

				if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Instrument) {
					InstrumentEventProcessorInterface* instrumentEventProcessor = dynamic_cast<InstrumentEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
					if (instrumentEventProcessor) {
						if (instrumentEventProcessor->GetStartTime() < currentTime && instrumentEventProcessor->GetIsTransferable()) {
							instrumentEventProcessor->FastForwardControlInstrument();
							//instrumentEventProcessor->SetIsTransfered();
						}
					}
					continue;
				}

				if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Time) {
					TimeEventProcessorInterface* timeEventProcessor = dynamic_cast<TimeEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
					if (timeEventProcessor) {
						if (timeEventProcessor->GetStartTime() < currentTime && timeEventProcessor->GetIsProcessable()) {
							timeEventProcessor->SetIsProcessed();
						}
					}
					continue;
				}
			}

			/* 由於playfield平移事件就算快轉也還是要執行，所以不用判斷time controller state */
			if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Playfield) {
				PlayfieldEventProcessorInterface* playfieldEventProcessor = dynamic_cast<PlayfieldEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
				if (playfieldEventProcessor) {
					if (playfieldEventProcessor->GetStartTime() < currentTime && playfieldEventProcessor->GetIsControllable()) {
						LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : found playfield event processor [" << playfieldEventProcessor->GetStartTime() << "].";
						playfieldEventProcessor->ControlPlayfield();
					}
				}
				continue;
			}

		}
	}

	/*
	 * 倒轉的狀態
	 */
	if (elapsedTime < 0) {

		eventProcessorPeriods->GetItemsContainPeriods(make_pair<float, float>((float)currentTime, currentTime - elapsedTime), &eventProcessors);
		// 移到遊戲最一開始就把所有event processor都先filter掉，這邊就只處理即時的filter
		LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent() : filter event processors.";
		if (eventProcessorFilter->GetIsGameTimeFiltering()) {

			filteredTempStaticEventProcessors.clear();
			for (int i = 0; i < eventProcessors.size(); i++) {
				if (eventProcessorFilter->GameTimeFilter(eventProcessors[i])) {
					filteredTempStaticEventProcessors.push_back(eventProcessors[i]);
				}
			}
		}
		else {
			filteredTempStaticEventProcessors.assign(eventProcessors.begin(), eventProcessors.end());
		}

		for (int i = 0; i < filteredTempStaticEventProcessors.size(); i++) {

			if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Playfield) {
				PlayfieldEventProcessorInterface* playfieldEventProcessor = dynamic_cast<PlayfieldEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
				if (playfieldEventProcessor) {
					if (playfieldEventProcessor->GetStartTime() > currentTime && playfieldEventProcessor->GetStartTime() < currentTime - elapsedTime) {
						LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : found playfield event processor [" << playfieldEventProcessor->GetStartTime() << "].";
						playfieldEventProcessor->UndoControlPlayfield();
					}
					continue;
				}
			}

			/* 如果是倒轉，就把這些event復原 */
			if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Io) {
				IoEventProcessorInterface* ioEventProcessors = dynamic_cast<IoEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
				if (ioEventProcessors) {
					if (ioEventProcessors->GetStartTime() > currentTime && ioEventProcessors->GetStartTime() < currentTime - elapsedTime) {
						ioEventProcessors->SetIsTransfered(false);
					}
				}
				continue;
			}

			if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Instrument) {
				InstrumentEventProcessorInterface* instrumentEventProcessor = dynamic_cast<InstrumentEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
				if (instrumentEventProcessor) {

					if (instrumentEventProcessor->GetStartTime() > currentTime && instrumentEventProcessor->GetStartTime() < currentTime - elapsedTime) {
						//instrumentEventProcessor->SetIsTransfered(false);
						instrumentEventProcessor->UndoControlInstrument();
					}
				}
				continue;
			}

			if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Time) {
				TimeEventProcessorInterface* timeEventProcessor = dynamic_cast<TimeEventProcessorInterface*>(filteredTempStaticEventProcessors[i]);
				if (timeEventProcessor) {
					if (timeEventProcessor->GetStartTime() > currentTime && timeEventProcessor->GetStartTime() < currentTime - elapsedTime) {
						timeEventProcessor->SetIsProcessed(false);
					}
				}
				continue;
			}
		}

	}



	/*
	 * dynamic events，目前只有instant模式會使用
	 */
	if (dynamicEventProcessors.size() == 0)
		return 0;

	/* 每次要用dynamic processors時，就要鎖起來 */
	lock_guard<mutex> guard(processorsMutex);
	for (int i = 0; i < dynamicEventProcessors.size(); i++) {

		LOG(LogLevel::Debug) << "EventProcessorMaster::processEvent : this processor is for [" << dynamicEventProcessors[i]->GetEvent()->GetTypeName() << "] [" << dynamicEventProcessors[i]->GetStartTime() << "]. " << currentTime;

		if (dynamicEventProcessors[i]->GetStartTime() >= currentTime)
			continue;

		if (dynamicEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Instrument) {
			InstrumentEventProcessorInterface* instrumentEventProcessor = dynamic_cast<InstrumentEventProcessorInterface*>(dynamicEventProcessors[i]);
			if (instrumentEventProcessor) {
				if (instrumentEventProcessor->GetStartTime() < currentTime && instrumentEventProcessor->GetIsTransferable()) {
					LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : found instrument event processor [" << instrumentEventProcessor->GetStartTime() << "].";
					instrumentEventProcessor->ControlInstrument();
				}
			}
			continue;
		}
		
		/* playfield事件 */
		if (dynamicEventProcessors[i]->GetEventProcessorType() == EventProcessorType::Playfield) {
			PlayfieldEventProcessorInterface* playfieldEventProcessor = dynamic_cast<PlayfieldEventProcessorInterface*>(dynamicEventProcessors[i]);
			if (playfieldEventProcessor) {
				if (playfieldEventProcessor->GetStartTime() < currentTime && playfieldEventProcessor->GetIsControllable()) {
					LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : found playfield event processor [" << playfieldEventProcessor->GetStartTime() << "].";
					playfieldEventProcessor->ControlPlayfield();
				}
			}
			continue;
		}
	}

	return 0;
}


Map * EventProcessorMaster::GetGraph()
{

	Map* graph = Drawable::GetGraph();

	graph->Reset();

	double currentTime = 0;
	try {
		currentTime = GetClock()->GetCurrentTime();
	}
	catch (exception& e) {
		LOG(LogLevel::Warning) << "EventProcessorMaster::GetGraph : clock is not started [" << e.what() << "].";
		return graph;
		//abort();
	}

	vector<EventProcessor<Event>*> eventProcessors;

	eventProcessors.assign(filteredTempStaticEventProcessors.begin(), filteredTempStaticEventProcessors.end());

	//eventProcessorPeriods->GetItemsContainPeriods(make_pair<float, float>(currentTime - visibleTimeRange, currentTime + visibleTimeRange), &eventProcessors);
	//LOG(LogLevel::Depricated) << "EventProcessorMaster::GetGraph : filter event processors.";
	//eventProcessorFilter->Filter(&eventProcessors);

	

	LOG(LogLevel::Depricated) << "EventProcessorMaster::GetGraph() : get graph from [" << eventProcessors.size() << "] processors in (" << currentTime - visibleTimeRange  << "," << currentTime + visibleTimeRange << ") seconds."
		<< [](vector<EventProcessor<Event>*>& eProcessors) {

		for (int i = 0; i < eProcessors.size(); i++)
			if(dynamic_cast<EffectMapperInterface*>(eProcessors[i]))
				LOG(LogLevel::Debug) << "---processor " << i << " time = [" << eProcessors[i]->GetStartTime() << "].";
		return 0;
	
	}(eventProcessors);

	for (int i = 0; i < eventProcessors.size(); i++) {
		EffectMapperInterface* effectMapper = dynamic_cast<EffectMapperInterface*>(eventProcessors[i]);
		if (effectMapper) {
			/* 檢查有沒有被濾掉 */
			if(eventProcessorFilter->GameTimeFilter(effectMapper))
				effectMapper->Draw(graph);
		}
	}

	LOG(LogLevel::Depricated) << "EventProcessorMaster::GetGraph : check if map writed." << [](int width, int height, Map* m) {
		bool isChanged = false;
		for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			if(m->Get(i, j) != 0)
				isChanged = true;
		if (isChanged) {
			LOG(LogLevel::Finer) << "EventProcessorMaster::GetGraph : light map - after";
			// 因為只看畫面中央，所以不看其他排
			for (int i = 0; i < width; i++) {
				string s;
				for (int j = 0; j < height; j++) {
					s += to_string(m->Get(i, j));
					s += " ";
				}
				LOG(LogLevel::Finer) << "| " << s << "|";
			}
		}
		return 0;
	}(graph->GetWidth(), graph->GetHeight(), graph);

	//return graph;

	if (dynamicEventProcessors.size() == 0)
		return graph;

	/* 每次要用dynamic processors時，就要鎖起來 */
	lock_guard<mutex> guard(processorsMutex);

	for (int i = 0; i < dynamicEventProcessors.size(); i++) {
		EffectMapperInterface* effectMapper = dynamic_cast<EffectMapperInterface*>(dynamicEventProcessors[i]);
		if (effectMapper) {
			LOG(LogLevel::Depricated) << "EventProcessorMaster::GetGraph : draw dynamic effect [" << effectMapper << "].";
			effectMapper->Draw(graph);
		}
	}
	
	//graph->PasteAdd(lightMap, 0, 0);
	return graph;
}

int EventProcessorMaster::update()
{
	if (isFirstUpdate) {
		isFirstUpdate = false;

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::StartGame);
		outputManager->PushMessage(meteoContextBluetoothMessage);
	}


	LOG(LogLevel::Finest) << "EventProcessorMaster::update() : current time is " << fixed << setprecision(5) << GetClock()->GetCurrentTime() << " .";


	// 這邊要檢查hit object有沒有miss的

	processEvent(GetClock()->GetElapsedFrameTime()); // 在這裡面處理io event

	LOG(LogLevel::Depricated) << "EventProcessorMaster::update() : Process event over.";

	if (dynamicEventProcessors.size() == 0)
		return 0;

	bool isDeleting = false;
	if(dynamicEventProcessors.size() > 0)
		LOG(LogLevel::Depricated) << "EventProcessorMaster::update : [" << dynamicEventProcessors.size() << "] dynamic processors.";

	/* 每次要用dynamic processors時，就要鎖起來 */
	lock_guard<mutex> guard(processorsMutex);

	/* 是件結束了就刪掉 */
	vector<EventProcessor<Event>*>::iterator iter;
	for (iter = dynamicEventProcessors.begin(); iter != dynamicEventProcessors.end(); ++iter) {

		bool thisOneNeedDelete = false;

		LOG(LogLevel::Depricated) << "EventProcessorMaster::update : step 1 get timed";

		if ((*iter)->GetProcessorLifeType() == EventLifeType::Timed &&
			(*iter)->GetTimeLeft() <= 0) {

			LOG(LogLevel::Debug) << "EventProcessorMaster::update : event [" << (*iter)->GetEventTypeName() << "] is to delete.";
			thisOneNeedDelete = true;
		}
		else if ((*iter)->GetProcessorLifeType() == EventLifeType::Immediate &&
				 (*iter)->GetIsProcessed()) {

			thisOneNeedDelete = true;
		}
		
		if (thisOneNeedDelete) {
			LOG(LogLevel::Depricated) << "EventProcessorMaster::update : step 2 erase.";
			if (!isDeleting) {
				/* 每次要用dynamic processors時，就要鎖起來 (用mutex就好，可以刪掉)*/
				isDeleting = true;
			}
			lock_guard<mutex> guard(processorsMutex);

			EventProcessor<Event>* ep = *iter;
			dynamicEventProcessors.erase(iter);
			iter--;

			

			LOG(LogLevel::Depricated) << "EventProcessorMaster::update : step 3 delete.";
			// TODO: 這邊會有thread safe的問題，要lock
			Event* e = ep->GetEvent();
			delete ep;
			if(e != nullptr)
				delete e;

		}
	}

	if (isDeleting) {
		LOG(LogLevel::Depricated) << "EventProcessorMaster::update : end.";
		//abort();
	}
	// TODO: 自動清除dynamic event，當調整時間或速度時，也把dynamic event清掉
	return 0;
}
