#ifndef PERIOD_MAP_H
#define PERIOD_MAP_H

#include <vector>
#include <map>
#include <functional>


using namespace std;


namespace Util {
namespace DataStructure {

	/// <summary>
	/// 在一段perioid裡面有哪些物件的map
	/// </summary>
	template<typename T>
	class PeriodMap {

		float insertionPoint;
		float periodLength;

		/// <summary>
		/// 將一整個map分成很多段，這樣蒐尋的時候就不用一段一段找
		/// </summary>
		multimap<pair<float, float>, T> periods;

		/// <summary>
		/// 用來把丟進去的event processor，抓出來他的時間區間是往前幾秒，往後幾秒
		/// </summary>
		function<pair<float, float>(T)> getTimeOfPeriod;

	public:
		PeriodMap(float iPoint, float pLength, function<pair<float, float>(T)> gTimeOfPeriod);



		int InsertItem(T item);

		int InsertItems(vector<T>* items);

		int DeleteItem(T item);

		int DeleteItem(pair<float, float> timeOfPeriod);

		/// <summary>
		/// 把所有區間和這個區間相比，有整段都在區間內就家進result裡
		/// </summary>
		int GetItemsInsidePeriods(pair<float, float> timeOfPeriod, vector<T>* results);

		/// <summary>
		/// 把所有區間和這個區間相比，有重疊到的話就家進去result裡
		/// </summary>
		int GetItemsContainPeriods(pair<float, float> timeOfPeriod, vector<T>* results);


		pair<float, float> GetPeriod(float timePoint);


	};


}}






#endif