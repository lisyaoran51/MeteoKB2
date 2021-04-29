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
		/// �@�w�|����A�p�G���\�Ϧ^��true�A�p�G�Q�R���N�^��false�C�|�b�C���}�l���e�N����
		/// </summary>
		vector<function<bool(EventProcessor<Event>*)>> filterCallbacks;

		/// <summary>
		/// �@�w�|����A�p�G���\�Ϧ^��true�A�p�G�Q�R���N�^��false�C�|�b�C���}�l���H�ɰ���
		/// </summary>
		vector<function<bool(EventProcessor<Event>*)>> dynamicFilterCallbacks;

		/// <summary>
		/// �u����ܬY�@variant�ɤ~�|����A�p�G���\�Ϧ^��true�A�p�G�Q�R���N�^��false
		/// </summary>
		multimap<int, function<bool(EventProcessor<Event>*)>> variantFilterCallbacks;

		/// <summary>
		/// �@�w�|����A�p�G���\�Ϧ^��true�A�p�G�Q�R���N�^��false
		/// </summary>
		map<string, function<bool(EventProcessor<Event>*)>> namedFilterCallbacks;

		int variant = -1;

		/// <summary>
		/// �ݭn�b�C������ɤ@��filter�A�o�OvariantFilterCallbacks�MnamedFilterCallbacks�~�ݭn��
		/// </summary>
		bool isGameTimeFiltering = false;

	public:

		EventProcessorFilter();

		int AddFilterCallback(function<bool(EventProcessor<Event>*)> filterCallback);

		int AddDynamicFilterCallback(function<bool(EventProcessor<Event>*)> dFilterCallback);
		
		int AddVariantFilterCallback(function<bool(EventProcessor<Event>*)> filterCallback, int v);

		int AddNamedFilterCallback(function<bool(EventProcessor<Event>*)> filterCallback, string name);

		bool GetIsGameTimeFiltering();

		/// <summary>
		/// -1�N��Ҧ�variant��������
		/// </summary>
		int SwitchVariant(int v);

		int GetVariant();

		int ClearFilterCallback();

		int ClearVariantFilterCallback(int v);

		int DeleteNamedFilterCallback(string name);

		/// <summary>
		/// �u�|����filterCallbacks
		/// </summary>
		bool Filter(EventProcessor<Event>* eventProcessor);

		/// <summary>
		/// �|����variantFilterCallbacks�MnamedFilterCallbacks�A�O�ΨӦb�C���i�椤�ϥ�
		/// </summary>
		bool GameTimeFilter(EventProcessor<Event>* eventProcessor);

		/// <summary>
		/// �|����variantFilterCallbacks(���Ӥ��|�o�˥�)
		/// </summary>
		bool Filter(EventProcessor<Event>* eventProcessor, int variant);

		/// <summary>
		/// �|����namedFilterCallbacks(���Ӥ��|�o�˥�)
		/// </summary>
		bool Filter(EventProcessor<Event>* eventProcessor, string name);


	};



}}}






#endif