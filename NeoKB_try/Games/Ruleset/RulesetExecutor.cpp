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
	
	// ��Event�নEvent processor�\�i�hplayfield��
	for (int i = 0; i < sm->events->size(); i++) {
		
		EventProcessor<Event>* ep = getEventProcessor(sm->events->at(i));

		playfield->Add(ep);

	}

	return 0;
}

/// <summary>
/// load�J�C�����A
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
	// ��pattern generator���W�r
	string pgName;
	
	if (!f->Get<string>(FrameworkSetting::PatternGenerator, &pgName))
		throw runtime_error("int RulesetExecutor<T>::load(FrameworkConfigManager*) : PatternGenerator not found in Setting.");

	// �Q��pattern generator���W�r�إ�pattern generator
	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
	PatternGenerator* pg = iCreator.CreateInstanceWithT<PatternGenerator>(pgName);

	// �n��converter�Mpostprocessor�\��load()�̡A�]��pattern Generator�O�\�bcache�̪�
	SmConverter* converter = createSmConverter(pg);
	SmPostprocessor* postprocessor = createSmPostprocessor();

	sm = converter->Convert(workingSm->GetSm());
	sm = postprocessor->postprocess(sm);

	delete converter;
	delete postprocessor;

	// Add mods, should always be the last thing applied to give full control to mods
	// applyMods(mods);

	playfield = createPlayfield();

	// �o��|��map algoŪ�i�hplayfield�̭��A�o��ƭn�O�o�g
	AddChild(playfield);

	// ��Event�নEvent processor�\�i�hplayfield��
	playfieldLoad();

	return 0;
}

template<class T>
RulesetExecutor<T>::RulesetExecutor()
{
	// ���Uprivate load (c++�~�ݭn)
	registerLoad(bind(static_cast<int(RulesetExecutor<T>::*)(void)>(&RulesetExecutor<T>::load), this));
}

template<class T>
int RulesetExecutor<T>::LazyConstruct(WorkingSm* w)
{
	workingSm = w;

	//mods = w->get_mods();

	return 0;
}





