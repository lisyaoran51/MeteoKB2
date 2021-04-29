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
		/// 一定會執行，如果允許救回傳true，如果被刪除就回傳false。會在遊戲開始之前就執行
		/// </summary>
		vector<function<bool(EventProcessor<Event>*)>> filterCallbacks;

		/// <summary>
		/// 一定會執行，如果允許救回傳true，如果被刪除就回傳false。會在遊戲開始後隨時執行
		/// </summary>
		vector<function<bool(EventProcessor<Event>*)>> dynamicFilterCallbacks;

		/// <summary>
		/// 只有選擇某一variant時才會執行，如果允許救回傳true，如果被刪除就回傳false
		/// </summary>
		multimap<int, function<bool(EventProcessor<Event>*)>> variantFilterCallbacks;

		/// <summary>
		/// 一定會執行，如果允許救回傳true，如果被刪除就回傳false
		/// </summary>
		map<string, function<bool(EventProcessor<Event>*)>> namedFilterCallbacks;

		int variant = -1;

		/// <summary>
		/// 需要在遊戲執行時一邊filter，這是variantFilterCallbacks和namedFilterCallbacks才需要的
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
		/// -1代表所有variant都不執行
		/// </summary>
		int SwitchVariant(int v);

		int GetVariant();

		int ClearFilterCallback();

		int ClearVariantFilterCallback(int v);

		int DeleteNamedFilterCallback(string name);

		/// <summary>
		/// 只會執行filterCallbacks
		/// </summary>
		bool Filter(EventProcessor<Event>* eventProcessor);

		/// <summary>
		/// 會執行variantFilterCallbacks和namedFilterCallbacks，是用來在遊戲進行中使用
		/// </summary>
		bool GameTimeFilter(EventProcessor<Event>* eventProcessor);

		/// <summary>
		/// 會執行variantFilterCallbacks(應該不會這樣用)
		/// </summary>
		bool Filter(EventProcessor<Event>* eventProcessor, int variant);

		/// <summary>
		/// 會執行namedFilterCallbacks(應該不會這樣用)
		/// </summary>
		bool Filter(EventProcessor<Event>* eventProcessor, string name);


	};



}}}






#endif