#ifndef EVENT_PROCESSOR_FILTER_H
#define EVENT_PROCESSOR_FILTER_H


#include <functional>
#include <vector>
#include <map>
#include "Event.h"
#include "../../../Framework/Allocation/Hierachal/Container.h"


using namespace std;
using namespace Framework::Allocation::Hierachal;




namespace Games {
namespace Schedulers {
namespace Events {

	template<class T>
	class EventProcessor;

	/*
	 * �Ψӧ⤣�ݭn��event processor�簣
	 * �bruleset executor�̥ͦ��A���O��ڤW�O��event processor master�ΡA�u�O�]���n���bruleset executor��modifier�ק�A�ҥH�����bruleset executor�ͦ�
	 * �o�˼g���Ǫ�
	 */
	/// <summary>
	/// �Ψӧ⤣�ݭn��event processor�簣
	/// �bruleset executor�̥ͦ��A���O��ڤW�O��event processor master�ΡA�u�O�]���n���bruleset executor��modifier�ק�A�ҥH�����bruleset executor�ͦ�
	/// �o�˼g���Ǫ�
	/// </summary>
	class EventProcessorFilter : public Container {

		int load();

		/// <summary>
		/// �@�w�|����
		/// </summary>
		vector<function<int(vector<EventProcessor<Event>*>*)>> filterCallbacks;

		/// <summary>
		/// �u����ܬY�@variant�ɤ~�|����
		/// </summary>
		multimap<int, function<int(vector<EventProcessor<Event>*>*)>> variantFilterCallbacks;

		/// <summary>
		/// �@�w�|����
		/// </summary>
		map<string, function<int(vector<EventProcessor<Event>*>*)>> namedFilterCallbacks;

		int variant = -1;

	public:

		EventProcessorFilter();

		int AddFilterCallback(function<int(vector<EventProcessor<Event>*>*)> filterCallback);
		
		int AddVariantFilterCallback(function<int(vector<EventProcessor<Event>*>*)> filterCallback, int v);

		int AddNamedFilterCallback(function<int(vector<EventProcessor<Event>*>*)> filterCallback, string name);

		/// <summary>
		/// -1�N��Ҧ�variant��������
		/// </summary>
		int SwitchVariant(int v);

		int GetVariant();

		int ClearFilterCallback();

		int ClearVariantFilterCallback(int v);

		int DeleteNamedFilterCallback(string name);

		/// <summary>
		/// �|����filterCallbacks�BvariantFilterCallbacks�BnamedFilterCallbacks
		/// </summary>
		vector<EventProcessor<Event>*>* Filter(vector<EventProcessor<Event>*>* eventProcessors);

		/// <summary>
		/// �|����variantFilterCallbacks(���Ӥ��|�o�˥�)
		/// </summary>
		vector<EventProcessor<Event>*>* Filter(vector<EventProcessor<Event>*>* eventProcessors, int variant);

		/// <summary>
		/// �|����namedFilterCallbacks(���Ӥ��|�o�˥�)
		/// </summary>
		vector<EventProcessor<Event>*>* Filter(vector<EventProcessor<Event>*>* eventProcessors, string name);


	};



}}}






#endif