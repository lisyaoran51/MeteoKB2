#ifndef PERIOD_MAP_H
#define PERIOD_MAP_H

#include <vector>
#include <map>
#include <functional>
#include <cmath>


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
		PeriodMap(float iPoint, float pLength, function<pair<float, float>(T)> gTimeOfPeriod) {
			insertionPoint = iPoint;
			periodLength = pLength;
			getTimeOfPeriod = gTimeOfPeriod;
		}



		int InsertItem(T item) {
			pair<float, float> insertTimeSpan = getTimeOfPeriod(item);

			// 例如 起始點3 區監5 物件時間7~13 => start=0, end=2
			int startSection = floor((insertTimeSpan.first - insertionPoint) / periodLength);
			int endSection = floor((insertTimeSpan.second - insertionPoint) / periodLength);

			for (int i = startSection; i <= endSection; i++) {
				// <3,8> <8,13> <13,18>
				periods.insert(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)), item);
			}

			return 0;
		}

		int InsertItems(vector<T>* items) {
			for (int i = 0; i < items->size(); i++) {
				InsertItem(items->at(i));
			}
			return 0;
		}

		int DeleteItem(T item) {
			typename multimap<pair<float, float>, T>::iterator it;
			for (it = periods.begin(); it != periods.end(); it++) {
				if (it->second == item) {
					periods.erase(it);
					it--;
				}
			}
			return 0;
		}

		int DeleteItem(pair<float, float> timeOfPeriod) {
			int startSection = floor((timeOfPeriod.first - insertionPoint) / periodLength);
			int endSection = floor((timeOfPeriod.second - insertionPoint) / periodLength);

			for (int i = startSection; i <= endSection; i++) {

				typename multimap<pair<float, float>, T>::iterator it = periods.find(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)));
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

		/// <summary>
		/// 把所有區間和這個區間相比，有整段都在區間內就家進result裡
		/// </summary>
		int GetItemsInsidePeriods(pair<float, float> timeOfPeriod, vector<T>* results) {
			int startSection = floor((timeOfPeriod.first - insertionPoint) / periodLength);
			int endSection = floor((timeOfPeriod.second - insertionPoint) / periodLength);

			for (int i = startSection; i <= endSection; i++) {

				typename multimap<pair<float, float>, T>::iterator it = periods.find(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)));
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

		/// <summary>
		/// 把所有區間和這個區間相比，有重疊到的話就家進去result裡
		/// </summary>
		int GetItemsContainPeriods(pair<float, float> timeOfPeriod, vector<T>* results) {
			int startSection = floor((timeOfPeriod.first - insertionPoint) / periodLength);
			int endSection = floor((timeOfPeriod.second - insertionPoint) / periodLength);

			for (int i = startSection; i <= endSection; i++) {

				typename multimap<pair<float, float>, T>::iterator it = periods.find(make_pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)));
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


		pair<float, float> GetPeriod(float timePoint) {
			int startSection = floor((timePoint - insertionPoint) / periodLength);
			return make_pair<float, float>(
				insertionPoint + periodLength * (float)startSection,
				insertionPoint + periodLength * (float)(startSection + 1));
		}



	};


}}






#endif