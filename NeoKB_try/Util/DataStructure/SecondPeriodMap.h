#ifndef SECOND_PERIOD_MAP_H
#define SECOND_PERIOD_MAP_H

#include "PeriodMap.h"
#include "SecondPeriod.h"




namespace Util {
namespace DataStructure {

	/// <summary>
	/// 在一段perioid裡面有哪些物件的map
	/// </summary>
	template<typename T>
	class SecondPeriodMap: public PeriodMap<T> {

		float insertionPoint;
		float periodLength;

		/// <summary>
		/// 將一整個map分成很多段，這樣蒐尋的時候就不用一段一段找
		/// 這個如果太慢，可以再改成vector，速度會變成O(1)，目前是O(logn)
		/// </summary>
		map<int, SecondPeriod<T>*> secondPeriods;

		/// <summary>
		/// 用來把丟進去的event processor，抓出來他的時間區間是往前幾秒，往後幾秒
		/// </summary>
		function<pair<float, float>(T)> getTimeOfPeriod;

	public:

		/// <summary>
		/// second period map跟period map的差別是，他固定insertion point是0，interval是1
		/// </summary>
		SecondPeriodMap(function<pair<float, float>(T)> gTimeOfPeriod): PeriodMap<T>(0, 1, gTimeOfPeriod){
		}

		virtual int InsertItem(T item) {
			pair<float, float> insertTimeSpan = getTimeOfPeriod(item);

			// 例如 起始點3 區監5 物件時間7~13 => start=0, end=2
			int startSection = floor(insertTimeSpan.first);
			int endSection = floor(insertTimeSpan.second);

			for (int i = startSection; i <= endSection; i++) {
				// <3,8> <8,13> <13,18>
				if (secondPeriods.find(i) != secondPeriods.end()) {
					secondPeriods[i] = new SecondPeriod<T>(i);
				}

				secondPeriods[i]->Add(item);
			}

			return 0;
		}

		virtual int DeleteItem(T item) {

			throw runtime_error("SecondPeriodMap::DeleteItem() : not implemented.");
			// not implemented
			return 0;
		}

		virtual int DeleteItem(pair<float, float> timeOfPeriod) {

			throw runtime_error("SecondPeriodMap::DeleteItem() : not implemented.");

			return 0;
		}

		/// <summary>
		/// 把所有區間和這個區間相比，有整段都在區間內就家進result裡
		/// </summary>
		int GetItemsInsidePeriods(pair<float, float> timeOfPeriod, vector<T>* results) {

			throw runtime_error("SecondPeriodMap::GetItemsInsidePeriods() : not implemented.");

			return 0;
		}

		/// <summary>
		/// 把所有區間和這個區間相比，有重疊到的話就家進去result裡
		/// </summary>
		virtual int GetItemsContainPeriods(pair<float, float> timeOfPeriod, vector<T>* results) {


			// 例如 起始點3 區監5 物件時間7~13 => start=0, end=2
			int startSection = floor(timeOfPeriod.first);
			int endSection = floor(timeOfPeriod.second);

			LOG(LogLevel::Finest) << "SecondPeriodMap::GetItemsContainPeriods() : period from [" << startSection << "] to [" << endSection << "] section.";

			for (int i = startSection; i <= endSection; i++) {

				for (int j = 0; j < secondPeriods[i]->objects.size(); j++) {

					results->push_back(secondPeriods[i]->objects[j]);
					
				}
			}
			return 0;
		}

		virtual pair<float, float> GetPeriod(float timePoint) {
			int startSection = floor(timePoint);
			return make_pair<float, float>(startSection, startSection + 1));
		}

		map<int, SecondPeriod<T>*>* GetSecondPeriods() {
			return &secondPeriods;
		}

	};


}}






#endif