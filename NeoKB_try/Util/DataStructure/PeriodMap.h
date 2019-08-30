#ifndef PERIOD_MAP_H
#define PERIOD_MAP_H

#include <vector>
#include <map>
#include <functional>


using namespace std;


namespace Util {
namespace DataStructure {

	/// <summary>
	/// �b�@�qperioid�̭������Ǫ���map
	/// </summary>
	template<typename T>
	class PeriodMap {

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
		PeriodMap(float iPoint, float pLength, function<pair<float, float>(T)> gTimeOfPeriod);



		int InsertItem(T item);

		int InsertItems(vector<T>* items);

		int DeleteItem(T item);

		int DeleteItem(pair<float, float> timeOfPeriod);

		/// <summary>
		/// ��Ҧ��϶��M�o�Ӱ϶��ۤ�A����q���b�϶����N�a�iresult��
		/// </summary>
		int GetItemsInsidePeriods(pair<float, float> timeOfPeriod, vector<T>* results);

		/// <summary>
		/// ��Ҧ��϶��M�o�Ӱ϶��ۤ�A�����|�쪺�ܴN�a�i�hresult��
		/// </summary>
		int GetItemsContainPeriods(pair<float, float> timeOfPeriod, vector<T>* results);


		pair<float, float> GetPeriod(float timePoint);


	};


}}






#endif