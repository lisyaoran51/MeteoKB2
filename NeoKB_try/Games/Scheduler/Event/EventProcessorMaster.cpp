#include "EventProcessorMaster.h"

#include "../../../Util/Log.h"
#include "Effect/EffectMapper.h"
#include <iomanip>
#include "HitObject.h"
#include "IoEvents/IoEventProcessor.h"
#include "InstrumentEvents/InstrumentEventProcessor.h"


using namespace Games::Schedulers::Events;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::IoEvents;
using namespace std;
using namespace Games::Schedulers::Events::InstrumentEvents;




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
	// TODO: �hframework config manager��period map�n�����h�e�@�q ex:�C5��@�q
	// �ثe���ȭq5��@�q
	int periodMapInterval = 3;
	if (!f->Get(FrameworkSetting::PeriodMapInterval, &periodMapInterval))
		periodMapInterval = 3;

	// TODO: visible time length�]���ӭn�hframework config manager��
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

	/* �C���n��dynamic processors�ɡA�N�n��_�� */
	lock_guard<mutex> guard(processorsMutex);

	dynamicEventProcessors.push_back(dEventProcessor);
	LOG(LogLevel::Debug) << "EventProcessorMaster::AddDynamicEventProcessor : add explosion [" << dEventProcessor << "], dynamic size = [" << dynamicEventProcessors.size() << "].";
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

	eventProcessorPeriods->GetItemsContainPeriods(make_pair<float, float>(currentTime - visibleTimeRange, currentTime + visibleTimeRange), &eventProcessors);

	for (int i = 0; i < eventProcessors.size(); i++) {

		LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : this processor is for [" << eventProcessors[i]->GetEvent()->GetTypeName() << "].";

		IoEventProcessorInterface* ioEventProcessors = dynamic_cast<IoEventProcessorInterface*>(eventProcessors[i]);
		if (ioEventProcessors) {
			if (ioEventProcessors->GetStartTime() < currentTime) {
				LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : found io event processor [" << ioEventProcessors->GetStartTime() << "].";
				ioEventProcessors->ProcessIo();
			}
			continue;
		}

		InstrumentEventProcessorInterface* instrumentEventProcessor = dynamic_cast<InstrumentEventProcessorInterface*>(eventProcessors[i]);
		if (instrumentEventProcessor) {
			if (instrumentEventProcessor->GetStartTime() > currentTime) {
				LOG(LogLevel::Depricated) << "EventProcessorMaster::processEvent : found instrument event processor [" << instrumentEventProcessor->GetStartTime() << "].";
				instrumentEventProcessor->ControlInstrument();
			}
			continue;
		}
	}

	// �o�䥻�ӬO�Ψӧ�sevent processor���ɶ��A���O�o�ˤӮ��O�į�A�令�Ҧ�processor������master��clock�hŪ�N�n

	//for (int i = 0; i < eventProcessors->size(); i++) {
	//	eventProcessors->at(i)->Elapse(elapsedTime);
	//}

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
			// �]���u�ݵe�������A�ҥH���ݨ�L��
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

	/* �C���n��dynamic processors�ɡA�N�n��_�� */
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

	LOG(LogLevel::Finest) << "EventProcessorMaster::update() : current time is " << fixed << setprecision(5) << GetClock()->GetCurrentTime() << " .";


	// �o��n�ˬdhit object���S��miss��

	processEvent(GetClock()->GetElapsedFrameTime()); // �b�o�̭��B�zio event

	//return 0;

	bool isDeleting = false;
	if(dynamicEventProcessors.size() > 0)
		LOG(LogLevel::Depricated) << "EventProcessorMaster::update : [" << dynamicEventProcessors.size() << "] dynamic processors.";

	/* �O�󵲧��F�N�R�� */
	vector<EventProcessor<Event>*>::iterator iter;
	for (iter = dynamicEventProcessors.begin(); iter != dynamicEventProcessors.end(); ++iter) {

		bool thisOneNeedDelete = false;

		LOG(LogLevel::Depricated) << "EventProcessorMaster::update : step 1 get timed";

		if ((*iter)->GetProcessorLifeType() == EventProcessorLifeType::Timed &&
			(*iter)->GetTimeLeft() <= 0) {

			thisOneNeedDelete = true;
		}
		else if ((*iter)->GetProcessorLifeType() == EventProcessorLifeType::Immediate &&
				 (*iter)->GetIsProcessed()) {

			thisOneNeedDelete = true;
		}
		
		if (thisOneNeedDelete) {
			LOG(LogLevel::Depricated) << "EventProcessorMaster::update : step 2 erase.";
			if (!isDeleting) {
				/* �C���n��dynamic processors�ɡA�N�n��_�� (��mutex�N�n�A�i�H�R��)*/
				isDeleting = true;
			}
			lock_guard<mutex> guard(processorsMutex);

			EventProcessor<Event>* ep = *iter;
			dynamicEventProcessors.erase(iter);
			iter--;

			LOG(LogLevel::Depricated) << "EventProcessorMaster::update : step 3 delete.";
			// TODO: �o��|��thread safe�����D�A�nlock
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
	// TODO: �۰ʲM��dynamic event�A��վ�ɶ��γt�׮ɡA�]��dynamic event�M��
	return 0;
}

int EventProcessorMaster::onKeyDown(InputState * inputState, Key key)
{
	/* ����Meteor Event Processor Master�̭��F */

	return 0;
}

