#ifndef SECOND_PERIOD_LIST_H
#define SECOND_PERIOD_LIST_H

#include "SecondPeriodMap.h"




namespace Util {
namespace DataStructure {

	/// <summary>
	/// 在一段perioid裡面有哪些物件的map
	/// </summary>
	template<typename T>
	class SecondPeriodList: public SecondPeriodMap<T> {

	protected:

		/// <summary>
		/// map太慢，改成vector，速度會變成O(1)
		/// </summary>
		vector<SecondPeriod<T>*> secondPeriodsInList;

	public:

		/// <summary>
		/// second period map跟period map的差別是，他固定insertion point是0，interval是1
		/// </summary>
		SecondPeriodList(function<pair<float, float>(T)> gTimeOfPeriod): SecondPeriodMap<T>(gTimeOfPeriod){
		}

		~SecondPeriodList() {
			for (int i = 0; i < secondPeriodsInList.size(); i++) {
				delete secondPeriodsInList[i];
			}
		}

		virtual int InsertItem(T item) {
			pair<float, float> insertTimeSpan = PeriodMap<T>::getTimeOfPeriod(item);

			// 例如 起始點3 區監5 物件時間7~13 => start=0, end=2
			int startSection = floor(insertTimeSpan.first);
			int endSection = floor(insertTimeSpan.second);

			for (int i = startSection; i <= endSection; i++) {
				// <3,8> <8,13> <13,18>
				if (secondPeriodsInList.size() <= i) {
					for (int j = secondPeriodsInList.size(); j <= i; j++) {
						secondPeriodsInList.push_back(new SecondPeriod<T>(j));
					}
				}

				secondPeriodsInList[i]->Add(item);
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

				results->reserve(secondPeriodsInList[i]->objects.size());
				results->insert(results->end(), secondPeriodsInList[i]->objects.begin(), secondPeriodsInList[i]->objects.end());

			}
			return 0;
		}


		vector<SecondPeriod<T>*>* GetSecondPeriodsInList() {
			return &secondPeriodsInList;
		}

	};


}}






#endif