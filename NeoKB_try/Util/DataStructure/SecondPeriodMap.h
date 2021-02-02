#ifndef SECOND_PERIOD_MAP_H
#define SECOND_PERIOD_MAP_H

#include "PeriodMap.h"
#include "SecondPeriod.h"




namespace Util {
namespace DataStructure {

	/// <summary>
	/// �b�@�qperioid�̭������Ǫ���map
	/// </summary>
	template<typename T>
	class SecondPeriodMap: public PeriodMap<T> {

		float insertionPoint;
		float periodLength;

		/// <summary>
		/// �N�@���map�����ܦh�q�A�o�˻`�M���ɭԴN���Τ@�q�@�q��
		/// �o�Ӧp�G�ӺC�A�i�H�A�令vector�A�t�׷|�ܦ�O(1)�A�ثe�OO(logn)
		/// </summary>
		map<int, SecondPeriod<T>*> secondPeriods;

		/// <summary>
		/// �Ψӧ��i�h��event processor�A��X�ӥL���ɶ��϶��O���e�X��A����X��
		/// </summary>
		function<pair<float, float>(T)> getTimeOfPeriod;

	public:

		/// <summary>
		/// second period map��period map���t�O�O�A�L�T�winsertion point�O0�Ainterval�O1
		/// </summary>
		SecondPeriodMap(function<pair<float, float>(T)> gTimeOfPeriod): PeriodMap<T>(0, 1, gTimeOfPeriod){
		}

		virtual int InsertItem(T item) {
			pair<float, float> insertTimeSpan = getTimeOfPeriod(item);

			// �Ҧp �_�l�I3 �Ϻ�5 ����ɶ�7~13 => start=0, end=2
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