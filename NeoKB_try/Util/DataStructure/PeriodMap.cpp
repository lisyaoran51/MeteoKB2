#include "PeriodMap.h"

#include <math.h>

using namespace Util::DataStructure;



template<typename T>
PeriodMap<T>::PeriodMap(float iPoint, float pLength, function<pair<float, float>(T)> gTimePoint)
{
	insertionPoint = iPoint;
	periodLength = pLength;
	getTimePoint = gTimePoint;
}

template<typename T>
int PeriodMap<T>::InsertItem(T item)
{
	pair<float, float> insertTimeSpan = getTimePoint(item);

	// 例如 起始點3 區監5 物件時間7~13 => start=0, end=2
	int startSection = floor((insertTimeSpan.first - insertionPoint) / interval);
	int endSection = floor((insertTimeSpan.second - insertionPoint) / interval);

	for (int i = startSection; i =< endSection; i++) {
		// <3,8> <8,13> <13,18>
		periods.insert(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i+1)), item);
	}

	return 0;
}

template<typename T>
int PeriodMap<T>::InsertItems(vector<T>* items)
{
	for (int i = 0; i < items->size(); i++) {
		InsertItem(items->at(i));
	}
	return 0;
}

template<typename T>
int PeriodMap<T>::DeleteItem(T item)
{
	multimap<pair<float, float>, T>::iterator it;
	for (it = periods.begin(); it != periods.end(); it++) {
		if (it->second == item) {
			periods.erase(it);
			it--;
		}
	}
	return 0;
}

template<typename T>
int PeriodMap<T>::DeleteItem(pair<float, float> timeOfPeriod)
{
	int startSection = floor((timeOfPeriod.first - insertionPoint) / interval);
	int endSection = floor((timeOfPeriod.second - insertionPoint) / interval);

	for (int i = startSection; i = < endSection; i++) {

		multimap<pair<float, float>, T>::iterator it = periods.find(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)));
		int count = periods.count(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)));
		
		for (int j = 0; j < count; j++, it++) {
			pair<float, float> timeSpan = getTimeOfPeriod(it->second);
			if (it->first.first <= timeSpan.first && it->first.second > timeSpan.second) {
				periods.erase(it);
				it--;
			}
		}
	}


	return 0;
}

template<typename T>
int PeriodMap<T>::GetItemsInsidePeriods(pair<float, float> timeOfPeriod, vector<T>* results)
{
	int startSection = floor((timeOfPeriod.first - insertionPoint) / interval);
	int endSection = floor((timeOfPeriod.second - insertionPoint) / interval);

	for (int i = startSection; i = < endSection; i++) {

		multimap<pair<float, float>, T>::iterator it = periods.find(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)));
		int count = periods.count(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)));

		for (int j = 0; j < count; j++, it++) {
			pair<float, float> timeSpan = getTimeOfPeriod(it->second);
			if (it->first.first <= timeSpan.first && it->first.second > timeSpan.second) {
				results->push_back(it->second);
			}
		}
	}

	return 0;
}

template<typename T>
int PeriodMap<T>::GetItemsContainPeriods(pair<float, float> timeOfPeriod, vector<T>* results)
{
	int startSection = floor((timeOfPeriod.first - insertionPoint) / interval);
	int endSection = floor((timeOfPeriod.second - insertionPoint) / interval);

	for (int i = startSection; i = < endSection; i++) {

		multimap<pair<float, float>, T>::iterator it = periods.find(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)));
		int count = periods.count(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)));

		for (int j = 0; j < count; j++, it++) {
			pair<float, float> timeSpan = getTimeOfPeriod(it->second);
			if (it->first.first <= timeSpan.first || it->first.second > timeSpan.second) {
				results->push_back(it->second);
			}
		}
	}
	return 0;
}

template<typename T>
pair<float, float> PeriodMap<T>::GetPeriod(float timePoint)
{
	int startSection = floor((timeOfPeriod.first - insertionPoint) / interval);
	int endSection = floor((timeOfPeriod.second - insertionPoint) / interval);
	return make_pair<float, float>(insertionPoint + periodLength * (float)i, insertionPoint + periodLength *  (float)(i + 1));
}
