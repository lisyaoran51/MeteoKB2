#include "EventProcessorMaster.h"

#include "../../../Util/Log.h"
#include "Effect/EffectMapper.h"


using namespace Games::Schedulers::Events;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;



int EventProcessorMaster::load()
{
	LOG(LogLevel::Finer) << "EventProcessorMaster::load() : getting FrameworkConfigManager";
	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int EventProcessorMaster::load() : FrameworkConfigManager not found in cache.");


	return load(f);
}

int EventProcessorMaster::load(FrameworkConfigManager * f)
{
	isPresent = true;
	// TODO: 去framework config manager拿period map要切成多寬一段 ex:每5秒一段
	// 目前先暫訂5秒一段
	int periodMapInterval = 3;
	if (!f->Get(FrameworkSetting::PeriodMapInterval, &periodMapInterval))
		periodMapInterval = 3;

	// TODO: visible time length也應該要去framework config manager拿
	eventProcessorPeriods = new PeriodMap<EventProcessor<Event>*>(0, periodMapInterval, [=](EventProcessor<Event>* ep)->pair<float, float> {

		float startTime = ep->GetStartTime() - visibleTimeRange;
		float endTime = ep->GetStartTime() + ep->GetLifeTime() + visibleTimeRange;

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


int EventProcessorMaster::processEvent(MTO_FLOAT elapsedTime)
{
	// 這邊本來是用來更新event processor的時間，但是這樣太浪費效能，改成所有processor直接拿master的clock去讀就好

	//for (int i = 0; i < eventProcessors->size(); i++) {
	//	eventProcessors->at(i)->Elapse(elapsedTime);
	//}

	return 0;
}


Map * EventProcessorMaster::GetGraph()
{

	Map* graph = Drawable::GetGraph();

	graph->Reset();

	double currentTime = GetClock()->GetCurrentTime();

	vector<EventProcessor<Event>*> eventProcessors;

	eventProcessorPeriods->GetItemsContainPeriods(make_pair<float, float>(currentTime - visibleTimeRange, currentTime + visibleTimeRange), &eventProcessors);
	

	LOG(LogLevel::Depricated) << "EventProcessorMaster::GetGraph() : get graph from [" << eventProcessors.size() << "] processors in (" << currentTime - visibleTimeRange  << "," << currentTime + visibleTimeRange << ") seconds."
		<< [](vector<EventProcessor<Event>*>& eProcessors) {

		for (int i = 0; i < eProcessors.size(); i++)
			LOG(LogLevel::Depricated) << "---processor " << i << " time = [" << eProcessors[i]->GetStartTime() << "].";
		return 0;
	
	}(eventProcessors);

	for (int i = 0; i < eventProcessors.size(); i++) {
		EffectMapperInterface* effectMapper = dynamic_cast<EffectMapperInterface*>(eventProcessors[i]);
		if (effectMapper) {
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

	/* 每次要用dynamic processors時，就要鎖起來 */
	lock_guard<mutex> guard(processorsMutex);

	for (int i = 0; i < dynamicEventProcessors.size(); i++) {
		EffectMapperInterface* effectMapper = dynamic_cast<EffectMapperInterface*>(dynamicEventProcessors[i]);
		if (effectMapper) {
			effectMapper->Draw(graph);
		}
	}
	
	//graph->PasteAdd(lightMap, 0, 0);
	return graph;
}

int EventProcessorMaster::update()
{

	processEvent(GetClock()->GetElapsedFrameTime()); // 這個是舊的程式，以後可能用不到了

	bool isDeleting = false;

	/* 是件結束了就刪掉 */
	vector<EventProcessor<Event>*>::iterator iter;
	for (iter = dynamicEventProcessors.begin(); iter != dynamicEventProcessors.end(); ++iter) {

		bool thisOneNeedDelete = false;

		if ((*iter)->GetProcessorLifeType() == EventProcessorLifeType::Timed &&
			(*iter)->GetProcessorTimeLeft() <= 0) {

			thisOneNeedDelete = true;
		}
		else if ((*iter)->GetProcessorLifeType() == EventProcessorLifeType::Immediate &&
				 (*iter)->GetIsProcessed()) {

			thisOneNeedDelete = true;
		}

		if (thisOneNeedDelete) {
			if (!isDeleting) {
				/* 每次要用dynamic processors時，就要鎖起來 */
				isDeleting = true;
				lock_guard<mutex> guard(processorsMutex);
			}

			EventProcessor<Event>* ep = *iter;
			dynamicEventProcessors.erase(iter);
			iter--;

			// TODO: 這邊會有thread safe的問題，要lock
			Event* e = ep->GetEvent();
			delete ep;
			delete e;
		}
	}


	// TODO: 自動清除dynamic event，當調整時間或速度時，也把dynamic event清掉
	return 0;
}

int EventProcessorMaster::onKeyDown(InputState * inputState, Key key)
{
	double currentTime = GetClock()->GetCurrentTime();

	vector<EventProcessor<Event>*> eventProcessors;

	eventProcessorPeriods->GetItemsContainPeriods(make_pair<float, float>(currentTime - visibleTimeRange, currentTime + visibleTimeRange), &eventProcessors);

	EventProcessor<Event>* receivedProcessor = nullptr;

	for (int i = 0; i < eventProcessors.size(); i++) {
		
		

	}


	return 0;
}

