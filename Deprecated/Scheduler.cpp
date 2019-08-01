#include "Scheduler.h"

#include "../../Util/Log.h"
#include "..\..\Framework\Allocation\Hierachal\Scheduler.h"


using namespace Base::Schedulers;
using namespace Util;




int Scheduler::load()
{
	Updater* u = GetCache<Updater>("Updater");
	if (!u)
		throw runtime_error("int EventProcessorMaster::load() : Updater not found in cache.");
	return load(u);
}

int Scheduler::load(Updater * u)
{
	LOG(LogLevel::Info) << "Scheduler::load(Updater*) : �Nschedule���ȵ��U��updater";

	u->RegisterTask(bind((int(Scheduler::*)(MTO_FLOAT))&Scheduler::Elapse, this, placeholders::_1));
	return 0;
}

Scheduler::Scheduler(): RegisterType("Scheduler"), ChildAddable()
{
	registerLoad(bind((int(Scheduler::*)())&Scheduler::load, this));
	eventProcessors = new vector<EventProcessor<Event>*>();
}

Scheduler::~Scheduler()
{
	delete eventProcessors;
}

int Scheduler::Add(EventProcessor<Event> * ep)
{
	eventProcessors->push_back(ep);

	return 0;
}

int Scheduler::AddRange(vector<EventProcessor<Event>*>* eps)
{
	for (int i = 0; i < eps->size(); i++) {
		Add(eps->at(i));
	}
	return 0;
}

int Scheduler::RegisterHandler(function<int(EventProcessor<Event>*)> dh)
{
	deliverHandler = dh;
	return 0;
}

int Scheduler::Elapse(MTO_FLOAT elapsedTime) {

	if (elapsedTime == -1) {
		// �C���٨S�}�l�ɨ�-1�A������0�N��C���}�l

		LOG(LogLevel::Finest) << [](vector<EventProcessor<Event>*>* eps) {
			for (int i = 0; i < eps->size(); i++) {
				LOG(LogLevel::Finest) << "int Scheduler::Elapse(MTO_FLOAT) : Processor #" << i << " start time is [" << eps->at(i)->GetStartTime() << "].";
			} 
			return 0;
		}(eventProcessors);

		currentTime = 0;
		return 0;
	}
	
	currentTime += elapsedTime;

	LOG(LogLevel::Finest) << "Scheduler::Elapse() : elapsed time - [" << elapsedTime << "], current time - [" << currentTime << "]";

	LOG(LogLevel::Finest) << "Scheduler::Elapse() : The start time of first event is [" << eventProcessors->front()->GetStartTime() << "].";

	if (eventProcessors->size() == 0)
		return 0;

	while (eventProcessors->front()->GetStartTime() < currentTime) {

		/* �Ȯɼg�k�A����n�ﱼ�A���M�|mem leak */
		if (eventProcessors->front()->GetStartTime() < 0) {
			LOG(LogLevel::Finer) << "Scheduler::Elapse() : event [" << eventProcessors->back()->GetStartTime() << "] is less than 0. Killed directly";

			eventProcessors->erase(eventProcessors->begin());
			continue;
		}

		LOG(LogLevel::Finer) << "Scheduler::Elapse() : event [" << eventProcessors->back()->GetStartTime() << "] popped.";

		//TODO: ���ӥ���W�L���ɶ�����^�h�A�קK�h��life time
		deliverHandler(eventProcessors->front());
		eventProcessors->erase(eventProcessors->begin());
	}
	return 0;
}
