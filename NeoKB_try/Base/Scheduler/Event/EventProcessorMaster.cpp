#include "EventProcessorMaster.h"

#include "../../../Util/Log.h"

using namespace Base::Schedulers::Events;
using namespace Util::Update;
using namespace Util;



int EventProcessorMaster::load()
{
	Updater* u = GetCache<Updater>("Updater");
	if (!u)
		throw runtime_error("int EventProcessorMaster::load() : Updater not found in cache.");
	return load(u);
}

int EventProcessorMaster::load(Updater * u)
{
	LOG(LogLevel::Info) << "EventProcessorMaster::load(Updater*) : 將process任務註冊至updater";

	u->RegisterTask(bind((int(EventProcessorMaster::*)(MTO_FLOAT))&EventProcessorMaster::Elapse, this, placeholders::_1));
	return 0;
}

EventProcessorMaster::EventProcessorMaster(): RegisterType("EventProcessorMaster"), ChildAddable(), Updatable()
{
	registerLoad(bind((int(EventProcessorMaster::*)())&EventProcessorMaster::load, this));
}

EventProcessorMaster::~EventProcessorMaster()
{
	delete eventProcessors;
}

int EventProcessorMaster::ReceiveEventProcessor(EventProcessor<Event>* ep)
{

	LOG(LogLevel::Finer) << "EventProcessorMaster::ReceiveEventProcessor(EventProcessor<Event>*) : event [" << ep->GetStartTime() << "] received.";

	eventProcessors->push_back(ep);
	return 0;
}

int EventProcessorMaster::RegisterMap(Map * m)
{
	lightMap = m;
	return 0;
}

int EventProcessorMaster::Clean()
{
	eventProcessors->clear();
	return 0;
}

int EventProcessorMaster::processEvent(MTO_FLOAT elapsedTime)
{
	for (int i = 0; i < eventProcessors->size(); i++) {
		eventProcessors->at(i)->Elapse(elapsedTime);
	}
	return 0;
}

int EventProcessorMaster::cleanEndedEvent()
{

	for (vector<EventProcessor<Event>*>::iterator i = eventProcessors->begin(); i != eventProcessors->end(); i++)
	{
		if ((*i)->GetTimeLeft() <= 0) {
			// TODO:
			// eventProcessors erase i 不知道怎麼寫
			i = eventProcessors->erase(i);
			// 音未刪除以後會直接跳向下一個元素，所以要先撿回來
			i--;
		}
	}

	return 0;
}

int EventProcessorMaster::Elapse(MTO_FLOAT elapsedTime)
{
	if (elapsedTime == -1) {
		currentTime = 0;
		return 0;
	}

	LOG(LogLevel::Finest) << "EventProcessorMaster::Elapse() : update frame - " << elapsedTime;

	// TODO: 判斷有沒有要render，沒有的話就簡化process event，不用寫入map
	processEvent(elapsedTime);
	cleanEndedEvent();
	return 0;
}

