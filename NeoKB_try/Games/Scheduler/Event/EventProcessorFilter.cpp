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

int EventProcessorFilter::AddFilterCallback(function<bool(EventProcessor<Event>*)> filterCallback)
{

	filterCallbacks.push_back(filterCallback);

	return 0;
}

int EventProcessorFilter::AddDynamicFilterCallback(function<bool(EventProcessor<Event>*)> dFilterCallback)
{
	dynamicFilterCallbacks.push_back(dFilterCallback);

	return 0;
}

int EventProcessorFilter::AddVariantFilterCallback(function<bool(EventProcessor<Event>*)> filterCallback, int v)
{
	variantFilterCallbacks.insert(pair<int, function<bool(EventProcessor<Event>*)>>(v, filterCallback));
	isGameTimeFiltering = true;
	return 0;
}

int EventProcessorFilter::AddNamedFilterCallback(function<bool(EventProcessor<Event>*)> filterCallback, string name)
{

	namedFilterCallbacks.insert(pair<string, function<bool(EventProcessor<Event>*)>>(name, filterCallback));
	isGameTimeFiltering = true;
	return 0;
}

bool EventProcessorFilter::GetIsGameTimeFiltering()
{
	return isGameTimeFiltering;
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

bool EventProcessorFilter::Filter(EventProcessor<Event>* eventProcessor)
{

	LOG(LogLevel::Depricated) << "EventProcessorFilter::Filter : filter callbacks." << filterCallbacks.size();
	for (int i = 0; i < filterCallbacks.size(); i++) {
		if (!filterCallbacks[i](eventProcessor))
			return false;
	}
	//LOG(LogLevel::Depricated) << "EventProcessorFilter::Filter : filter variant callbacks." << variantFilterCallbacks.size();
	//for (multimap<int, function<bool(EventProcessor<Event>*)>>::iterator i = variantFilterCallbacks.begin(); i != variantFilterCallbacks.end(); i++) {
	//	if ((*i).first == variant)
	//		if (!(*i).second(eventProcessor)) {
	//			return false;
	//		}
	//}
	//LOG(LogLevel::Depricated) << "EventProcessorFilter::Filter : filter named callbacks." << namedFilterCallbacks.size();
	//for (map<string, function<bool(EventProcessor<Event>*)>>::iterator i = namedFilterCallbacks.begin(); i != namedFilterCallbacks.end(); i++) {
	//	
	//	if (!(*i).second(eventProcessor)) {
	//		return false;
	//	}
	//}

	return true;;
}

bool EventProcessorFilter::GameTimeFilter(EventProcessor<Event>* eventProcessor)
{
	for (int i = 0; i < dynamicFilterCallbacks.size(); i++) {
		if (!dynamicFilterCallbacks[i](eventProcessor))
			return false;
	}

	LOG(LogLevel::Depricated) << "EventProcessorFilter::Filter : filter variant callbacks." << variantFilterCallbacks.size();
	for (multimap<int, function<bool(EventProcessor<Event>*)>>::iterator i = variantFilterCallbacks.begin(); i != variantFilterCallbacks.end(); i++) {
		if ((*i).first == variant)
			if (!(*i).second(eventProcessor)) {
				return false;
			}
	}
	LOG(LogLevel::Depricated) << "EventProcessorFilter::Filter : filter named callbacks." << namedFilterCallbacks.size();
	for (map<string, function<bool(EventProcessor<Event>*)>>::iterator i = namedFilterCallbacks.begin(); i != namedFilterCallbacks.end(); i++) {
		
		if (!(*i).second(eventProcessor)) {
			return false;
		}
	}
	return true;
}

bool EventProcessorFilter::Filter(EventProcessor<Event>* eventProcessor, int v)
{
	for (multimap<int, function<bool(EventProcessor<Event>*)>>::iterator i = variantFilterCallbacks.begin(); i != variantFilterCallbacks.end(); i++) {
		if ((*i).first == v)
			if (!(*i).second(eventProcessor)) {
				return false;
			}
	}

	return true;
}

bool EventProcessorFilter::Filter(EventProcessor<Event>* eventProcessor, string name)
{
	for (map<string, function<bool(EventProcessor<Event>*)>>::iterator i = namedFilterCallbacks.begin(); i != namedFilterCallbacks.end(); i++) {
		if ((*i).first == name)
			if (!(*i).second(eventProcessor)) {
				return false;
			}
	}
	return true;
}
