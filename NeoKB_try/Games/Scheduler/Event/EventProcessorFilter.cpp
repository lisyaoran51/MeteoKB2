#include "EventProcessorFilter.h"


using namespace std;
using namespace Games::Schedulers::Events;



int EventProcessorFilter::load()
{
	GetDependencies()->Cache(this);
	return 0;
}

EventProcessorFilter::EventProcessorFilter(): RegisterType("EventProcessorFilter")
{
	registerLoad(bind(static_cast<int(EventProcessorFilter::*)(void)>(&EventProcessorFilter::load), this));
}

int EventProcessorFilter::AddFilterCallback(function<int(vector<EventProcessor<Event>*>*)> filterCallback)
{

	filterCallbacks.push_back(filterCallback);

	return 0;
}

int EventProcessorFilter::AddVariantFilterCallback(function<int(vector<EventProcessor<Event>*>*)> filterCallback, int v)
{
	variantFilterCallbacks.insert(pair<int, function<int(vector<EventProcessor<Event>*>*)>>(v, filterCallback));

	return 0;
}

int EventProcessorFilter::AddNamedFilterCallback(function<int(vector<EventProcessor<Event>*>*)> filterCallback, string name)
{

	namedFilterCallbacks.insert(pair<string, function<int(vector<EventProcessor<Event>*>*)>>(name, filterCallback));
	return 0;
}

int EventProcessorFilter::SwitchVariant(int v)
{
	variant = v;
	return 0;
}

int EventProcessorFilter::GetVariant()
{
	return variant;
}

int EventProcessorFilter::ClearFilterCallback()
{
	filterCallbacks.clear();
	return 0;
}

int EventProcessorFilter::ClearVariantFilterCallback(int variant)
{
	// TODO: 清掉該variant的multimap元素
	return 0;
}

int EventProcessorFilter::DeleteNamedFilterCallback(string name)
{
	// TODO: 清掉該name的map元素
	return 0;
}

vector<EventProcessor<Event>*>* EventProcessorFilter::Filter(vector<EventProcessor<Event>*>* eventProcessors)
{
	LOG(LogLevel::Finer) << "EventProcessorFilter::Filter : filter callbacks." << filterCallbacks.size();
	for (int i = 0; i < filterCallbacks.size(); i++) {
		filterCallbacks[i](eventProcessors);
	}
	LOG(LogLevel::Finer) << "EventProcessorFilter::Filter : filter variant callbacks." << variantFilterCallbacks.size();
	for (multimap<int, function<int(vector<EventProcessor<Event>*>*)>>::iterator i = variantFilterCallbacks.begin(); i != variantFilterCallbacks.end(); i++) {
		if ((*i).first == variant)
			(*i).second(eventProcessors);
	}
	LOG(LogLevel::Finer) << "EventProcessorFilter::Filter : filter named callbacks." << namedFilterCallbacks.size();
	for (map<string, function<int(vector<EventProcessor<Event>*>*)>>::iterator i = namedFilterCallbacks.begin(); i != namedFilterCallbacks.end(); i++) {
		
		(*i).second(eventProcessors);
	}

	return eventProcessors;
}

vector<EventProcessor<Event>*>* EventProcessorFilter::Filter(vector<EventProcessor<Event>*>* eventProcessors, int v)
{
	for (multimap<int, function<int(vector<EventProcessor<Event>*>*)>>::iterator i = variantFilterCallbacks.begin(); i != variantFilterCallbacks.end(); i++) {
		if ((*i).first == v)
			(*i).second(eventProcessors);
	}

	return eventProcessors;
}

vector<EventProcessor<Event>*>* EventProcessorFilter::Filter(vector<EventProcessor<Event>*>* eventProcessors, string name)
{
	for (map<string, function<int(vector<EventProcessor<Event>*>*)>>::iterator i = namedFilterCallbacks.begin(); i != namedFilterCallbacks.end(); i++) {
		if ((*i).first == name)
			(*i).second(eventProcessors);
	}
	return eventProcessors;
}
