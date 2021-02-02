#ifndef PERIOD_MAP_H
#define PERIOD_MAP_H

#include <vector>
#include <map>
#include <functional>
#include <cmath>
#include "../Log.h"


using namespace std;
using namespace Util;


namespace Util {
namespace DataStructure {

	/// <summary>
	/// �b�@�qperioid�̭������Ǫ���map
	/// </summary>
	template<typename T>
	class PeriodMap {

	protected:

		float insertionPoint;
		float periodLength;

		/// <summary>
		/// �N�@���map�����ܦh�q�A�o�˻`�M���ɭԴN���Τ@�q�@�q��
		/// </summary>
		multimap<pair<float, float>, T> periods;

		/// <summary>
		/// �Ψӧ��i�h��event processor�A��X�ӥL���ɶ��϶��O���e�X��A����X��
		/// </summary>
		function<pair<float, float>(T)> getTimeOfPeriod;

	public:
		PeriodMap(float iPoint, float pLength, function<pair<float, float>(T)> gTimeOfPeriod) {
			insertionPoint = iPoint;
			periodLength = pLength;
			getTimeOfPeriod = gTimeOfPeriod;
		}



		virtual int InsertItem(T item) {
			pair<float, float> insertTimeSpan = getTimeOfPeriod(item);

			// �Ҧp �_�l�I3 �Ϻ�5 ����ɶ�7~13 => start=0, end=2
			int startSection = floor((insertTimeSpan.first - insertionPoint) / periodLength);
			int endSection = floor((insertTimeSpan.second - insertionPoint) / periodLength);

			for (int i = startSection; i <= endSection; i++) {
				// <3,8> <8,13> <13,18>
				periods.insert(pair<pair<float, float>, T>(pair<float, float>(insertionPoint + periodLength *  (float)i, insertionPoint + periodLength *  (float)(i + 1)), item));
			}

			return 0;
		}

		int InsertItems(vector<T>* items) {
			for (int i = 0; i < items->size(); i++) {
				InsertItem(items->at(i));
			}
			return 0;
		}

		virtual int DeleteItem(T item) {
			typename multimap<pair<float, float>, T>::iterator it;
			for (it = periods.begin(); it != periods.end(); it++) {
				if (it->second == item) {
					periods.erase(it);
					it--;
				}
			}
			return 0;
		}

		virtual int DeleteItem(pair<float, float> timeOfPeriod) {
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
		/// ��Ҧ��϶��M�o�Ӱ϶��ۤ�A����q���b�϶����N�a�iresult��
		/// </summary>
		virtual int GetItemsInsidePeriods(pair<float, float> timeOfPeriod, vector<T>* results) {
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
		/// ��Ҧ��϶��M�o�Ӱ϶��ۤ�A�����|�쪺�ܴN�a�i�hresult��
		/// </summary>
		virtual int GetItemsContainPeriods(pair<float, float> timeOfPeriod, vector<T>* results) {
			int startSection = floor((timeOfPeriod.first - insertionPoint) / periodLength);
			int endSection = floor((timeOfPeriod.second - insertionPoint) / periodLength);

			LOG(LogLevel::Finest) << "PeriodMap::GetItemsContainPeriods() : period from [" << startSection << "] to [" << endSection << "] section.";

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


		virtual pair<float, float> GetPeriod(float timePoint) {
			int startSection = floor((timePoint - insertionPoint) / periodLength);
			return make_pair<float, float>(
				insertionPoint + periodLength * (float)startSection,
				insertionPoint + periodLength * (float)(startSection + 1));
		}

		multimap<pair<float, float>, T>* GetPeriods() {
			return &periods;
		}

		multimap<pair<float, float>, T>* GetMultiMapPeriods() {
			return &periods;
		}

	};


}}






#endif