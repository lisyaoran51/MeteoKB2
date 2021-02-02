#ifndef SECOND_PERIOD_LIST_H
#define SECOND_PERIOD_LIST_H

#include "SecondPeriodMap.h"




namespace Util {
namespace DataStructure {

	/// <summary>
	/// �b�@�qperioid�̭������Ǫ���map
	/// </summary>
	template<typename T>
	class SecondPeriodList: public SecondPeriodMap<T> {

	protected:

		/// <summary>
		/// map�ӺC�A�令vector�A�t�׷|�ܦ�O(1)
		/// </summary>
		vector<SecondPeriod<T>*> secondPeriodsInList;

	public:

		/// <summary>
		/// second period map��period map���t�O�O�A�L�T�winsertion point�O0�Ainterval�O1
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

			// �Ҧp �_�l�I3 �Ϻ�5 ����ɶ�7~13 => start=0, end=2
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
		/// ��Ҧ��϶��M�o�Ӱ϶��ۤ�A����q���b�϶����N�a�iresult��
		/// </summary>
		int GetItemsInsidePeriods(pair<float, float> timeOfPeriod, vector<T>* results) {

			throw runtime_error("SecondPeriodMap::GetItemsInsidePeriods() : not implemented.");

			return 0;
		}

		/// <summary>
		/// ��Ҧ��϶��M�o�Ӱ϶��ۤ�A�����|�쪺�ܴN�a�i�hresult��
		/// </summary>
		virtual int GetItemsContainPeriods(pair<float, float> timeOfPeriod, vector<T>* results) {


			// �Ҧp �_�l�I3 �Ϻ�5 ����ɶ�7~13 => start=0, end=2
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