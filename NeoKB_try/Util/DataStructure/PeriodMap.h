#ifndef PERIOD_MAP_H
#define PERIOD_MAP_H

#include <vector>
#include <map>
#include <functional>


using namespace std;


namespace Util {
namespace DataStructure {

	template<typename T>
	class PeriodMap {

		float insertionPoint;
		float periodLength;

		multimap<pair<float, float>, T> periods;

		function<pair<float, float>(T)> getTimeOfPeriod;

	public:
		PeriodMap(float iPoint, float i, function<pair<float, float>(T)> gTimeOfPeriod);

		int InsertItem(T item);

		int InsertItems(vector<T>* items);

		int DeleteItem(T item);

		int DeleteItem(pair<float, float> timeOfPeriod);

		int GetItemsInsidePeriods(pair<float, float> timeOfPeriod, vector<T>* results);

		int GetItemsContainPeriods(pair<float, float> timeOfPeriod, vector<T>* results);


		pair<float, float> GetPeriod(float timePoint);


	};


}}






#endif