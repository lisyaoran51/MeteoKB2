#include "EventProcessorMaster.h"

#include "../../../Util/Log.h"
#include "Effect\EffectMapper.h"

using namespace Base::Schedulers::Events;
using namespace Util;
using namespace Base::Schedulers::Events::Effects;



int EventProcessorMaster::load()
{
	return 0;
}


EventProcessorMaster::EventProcessorMaster(): RegisterType("EventProcessorMaster"), Container()
{
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

	return 0;
}

int EventProcessorMaster::AddDynamicEventProcessor(EventProcessor<Event>* dEventProcessor)
{
	dEventProcessor->Attach(this);
	dynamicEventProcessors.push_back(dEventProcessor);
	return 0;
}

int EventProcessorMaster::RegisterMap(Map * m)
{
	lightMap = m;
	return 0;
}

int EventProcessorMaster::Clean()
{
	//eventProcessors->clear();
	return 0;
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
	
	for (int i = 0; i < eventProcessors.size(); i++) {
		EffectMapperInterface* effectMapper = dynamic_cast<EffectMapperInterface*>(eventProcessors[i]);
		if (effectMapper) {
			effectMapper->Draw(lightMap);
		}
	}
	

	graph->PasteAdd(lightMap, 0, 0);
	return nullptr;
}

int EventProcessorMaster::update()
{

	processEvent(GetClock()->GetElapsedFrameTime());

	// TODO: 自動清除dynamic event，當調整時間或速度時，也把dynamic event清掉
	return 0;
}

