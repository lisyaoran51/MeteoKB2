#include "RulesetExecutor.h"
#include "../../Util/MtoObject.h"


using namespace Base::Rulesets;
using namespace Util;
using namespace Base::Sheetmusics;
using namespace Base::Sheetmusics::Patterns;
using namespace Base::Schedulers::Events;


template<class T>
int RulesetExecutor<T>::playfieldLoad()
{
	
	// 把Event轉成Event processor擺進去playfield裡
	for (int i = 0; i < sm->events->size(); i++) {
		
		EventProcessor<Event>* ep = getEventProcessor(sm->events->at(i));

		playfield->Add(ep);

	}

	return 0;
}

/// <summary>
/// load入遊戲狀態
/// </summary>
template<class T>
int RulesetExecutor<T>::load()
{
	FrameworkConfigManager * f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int  RulesetExecutor<T>::load() : FrameworkConfigManager not found in cache.");

	return load(f);
}

template<class T>
int RulesetExecutor<T>::load(FrameworkConfigManager* f)
{
	// 取pattern generator的名字
	string pgName;
	
	if (!f->Get<string>(FrameworkSetting::PatternGenerator, &pgName))
		throw runtime_error("int RulesetExecutor<T>::load(FrameworkConfigManager*) : PatternGenerator not found in Setting.");

	// 利用pattern generator的名字建立pattern generator
	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
	PatternGenerator* pg = iCreator.CreateInstanceWithT<PatternGenerator>(pgName);

	// 要把converter和postprocessor擺到load()裡，因為pattern Generator是擺在cache裡的
	SmConverter* converter = createSmConverter(pg);
	SmPostprocessor* postprocessor = createSmPostprocessor();

	sm = converter->Convert(workingSm->GetSm());
	sm = postprocessor->postprocess(sm);

	delete converter;
	delete postprocessor;

	// Add mods, should always be the last thing applied to give full control to mods
	// applyMods(mods);

	playfield = createPlayfield();

	// 這邊會把map algo讀進去playfield裡面，這件事要記得寫
	AddChild(playfield);

	// 把Event轉成Event processor擺進去playfield裡
	playfieldLoad();

	return 0;
}

template<class T>
RulesetExecutor<T>::RulesetExecutor()
{
	// 註冊private load (c++才需要)
	registerLoad(bind(static_cast<int(RulesetExecutor<T>::*)(void)>(&RulesetExecutor<T>::load), this));
}

template<class T>
int RulesetExecutor<T>::LazyConstruct(WorkingSm* w)
{
	workingSm = w;

	//mods = w->get_mods();

	return 0;
}





