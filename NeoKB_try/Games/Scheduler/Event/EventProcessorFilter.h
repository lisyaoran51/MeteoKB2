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
	 * 用來把不需要的event processor剔除
	 * 在ruleset executor裡生成，但是實際上是給event processor master用，只是因為要先在ruleset executor讓modifier修改，所以提早在ruleset executor生成
	 * 這樣寫滿怪的
	 */
	/// <summary>
	/// 用來把不需要的event processor剔除
	/// 在ruleset executor裡生成，但是實際上是給event processor master用，只是因為要先在ruleset executor讓modifier修改，所以提早在ruleset executor生成
	/// 這樣寫滿怪的
	/// </summary>
	class EventProcessorFilter : public Container {

		int load();

		/// <summary>
		/// 一定會執行
		/// </summary>
		vector<function<int(vector<EventProcessor<Event>*>*)>> filterCallbacks;

		/// <summary>
		/// 只有選擇某一variant時才會執行
		/// </summary>
		multimap<int, function<int(vector<EventProcessor<Event>*>*)>> variantFilterCallbacks;

		/// <summary>
		/// 一定會執行
		/// </summary>
		map<string, function<int(vector<EventProcessor<Event>*>*)>> namedFilterCallbacks;

		int variant = -1;

	public:

		EventProcessorFilter();

		int AddFilterCallback(function<int(vector<EventProcessor<Event>*>*)> filterCallback);
		
		int AddVariantFilterCallback(function<int(vector<EventProcessor<Event>*>*)> filterCallback, int v);

		int AddNamedFilterCallback(function<int(vector<EventProcessor<Event>*>*)> filterCallback, string name);

		/// <summary>
		/// -1代表所有variant都不執行
		/// </summary>
		int SwitchVariant(int v);

		int GetVariant();

		int ClearFilterCallback();

		int ClearVariantFilterCallback(int v);

		int DeleteNamedFilterCallback(string name);

		/// <summary>
		/// 會執行filterCallbacks、variantFilterCallbacks、namedFilterCallbacks
		/// </summary>
		vector<EventProcessor<Event>*>* Filter(vector<EventProcessor<Event>*>* eventProcessors);

		/// <summary>
		/// 會執行variantFilterCallbacks(應該不會這樣用)
		/// </summary>
		vector<EventProcessor<Event>*>* Filter(vector<EventProcessor<Event>*>* eventProcessors, int variant);

		/// <summary>
		/// 會執行namedFilterCallbacks(應該不會這樣用)
		/// </summary>
		vector<EventProcessor<Event>*>* Filter(vector<EventProcessor<Event>*>* eventProcessors, string name);


	};



}}}






#endif