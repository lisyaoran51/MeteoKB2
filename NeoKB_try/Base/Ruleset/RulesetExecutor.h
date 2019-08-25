#ifndef RULESET_EXECUTOR_H
#define RULESET_EXECUTOR_H


#include "RulesetInfo.h"
#include "../../Util/TemplateConstraint.h"
#include "../Scheduler/Event/Event.h"
#include "../Scheduler/Event/EventProcessor.h"
#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../Sheetmusic/SheetmusicConverter.h"
#include "../Sheetmusic/SheetmusicPostprocessor.h"
#include "../Scene/Play/Playfield.h"
#include "../Sheetmusic/Pattern/PatternGenerator.h"
#include "../../Framework/Configurations/FrameworkConfigManager.h"
#include "../Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../Sheetmusic/WorkingSheetmusic.h"



/*
 * virutal private���n�B�A�i�H���l���h�ק�����A���S����ȱ�call����
 * https://stackoverflow.com/questions/2170688/private-virtual-method-in-c
 */



using namespace std;
using namespace Util;
using namespace Base::Schedulers::Events;
using namespace Base::Sheetmusics;
using namespace Base::Sheetmusics::Patterns;
using namespace Framework::Configurations;
using namespace Base::Schedulers::Events::Effects::Algorithms;
using namespace Base::Rulesets;
using namespace Framework::Allocation::Hierachal;
using namespace Base::Scenes::Play;


namespace Base {
namespace Rulesets {
	
	/// <summary>
	/// a game rule that designs how the game plays
	///	</summary>
	template<class T>
	class RulesetExecutor: private TConstraint<T, Event>, public Container
	{

		/// <summary>
		/// �P�_�o�Ӫ��ئ��S���Q�غc�A�غc��~�i�ϥ�
		///	</summary>
		bool constructed;

		/// <summary>
		/// jobs:
		/// 1. find the objects in sm?
		/// 2. add them to playfield?
		/// </summary>
		int playfieldLoad() {

			LOG(LogLevel::Fine) << "RulesetExecutor::playfieldLoad : loading [" << sm->GetEvents()->size() << "] events into playfield ... ";

			// ��Event�নEvent processor�\�i�hplayfield��
			for (int i = 0; i < sm->GetEvents()->size(); i++) {

				EventProcessor<Event>* ep = getEventProcessor(sm->GetEvents()->at(i));
				playfield->Add(ep);
			}

			return 0;
		}

		/// <summary>
		/// jobs:
		/// 1. create playfield
		/// 2. add playfield as child
		/// 3. ??? load objects?
		/// </summary>
		int load() {

			LOG(LogLevel::Info) << "RulesetExecutor::load : start loading the executor and building the sm and playfield.";

			FrameworkConfigManager * f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
			if (!f)
				throw runtime_error("int  RulesetExecutor<T>::load() : FrameworkConfigManager not found in cache.");

			return load(f);
		}

		/// <summary>
		/// load�J�C�����A
		/// </summary>
		int load(FrameworkConfigManager* f) {
			// ��pattern generator���W�r
			string pgName;

			if (!f->Get(FrameworkSetting::PatternGenerator, &pgName))
				throw runtime_error("int RulesetExecutor<T>::load(FrameworkConfigManager*) : PatternGenerator not found in Setting.");

			// �Q��pattern generator���W�r�إ�pattern generator
			LOG(LogLevel::Info) << "RulesetExecutor::load : creating [" << pgName << "] ...";
			InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
			PatternGenerator* pg = iCreator.CreateInstanceWithT<PatternGenerator>(pgName);
			AddChild(pg);

			// �n��converter�Mpostprocessor�\��load()�̡A�]��pattern Generator�O�\�bcache�̪�
			LOG(LogLevel::Fine) << "RulesetExecutor::load : creating Sm Converter ...";
			SmConverter* converter = createSmConverter(pg);
			SmPostprocessor* postprocessor = createSmPostprocessor();


			LOG(LogLevel::Fine) << "RulesetExecutor::load : Converting sm ...";
			sm = converter->Convert(workingSm->GetSm());
			sm = postprocessor->postprocess(sm);

			for (int i = 0; i < sm->GetEvents()->size(); i++) {
				LOG(LogLevel::Finest) << "RulesetExecutor::load : loaded event #" << i << " start time is [" << sm->GetEvents()->at(i)->GetStartTime() << "].";
			}

			delete converter;
			delete postprocessor;

			// Add mods, should always be the last thing applied to give full control to mods
			// applyMods(mods);

			LOG(LogLevel::Info) << "RulesetExecutor::load : creating playfield ...";
			playfield = createPlayfield();

			// �o��|��map algoŪ�i�hplayfield�̭��A�o��ƭn�O�o�g
			AddChild(playfield);

			// ��Event�নEvent processor�\�i�hplayfield��
			LOG(LogLevel::Info) << "RulesetExecutor::load : loading events into playfield ...";
			playfieldLoad();

			return 0;
		}

		//int applyMods(vector<mod*>* m);

		virtual SmConverter* createSmConverter(PatternGenerator* pg) = 0;

		virtual SmPostprocessor* createSmPostprocessor() = 0;

	public:

		/// <summary>
		/// jobs:
		/// 1. create sm from working sm
		/// 2. apply mods
		/// 3. register privateLoad()
		/// </summary>
		RulesetExecutor(): RegisterType("RulesetExecutor") {
			// ���Uprivate load (c++�~�ݭn)
			registerLoad(bind(static_cast<int(RulesetExecutor<T>::*)(void)>(&RulesetExecutor<T>::load), this));
			constructed = false;
		}

		virtual ~RulesetExecutor() = default;

		virtual int LazyConstruct(WorkingSm* w) {

			workingSm = w;

			//mods = w->get_mods();

			constructed = true;

			return 0;
		}


	protected:

		Sm<T>* sm = NULL;

		WorkingSm* workingSm = NULL;

		//vector<mod_t*> mods;

		Playfield* playfield = NULL;

		vector<void*> on_judgement;

		virtual Playfield* createPlayfield() = 0;

		/// <summary>
		/// jobs:
		/// 1. �إ߻Pevent������processor
		/// 2. �p�Gprocessor�Oeffect mapper�A�N��map algo�[�i�h
		/// </summary>
		virtual EventProcessor<T>* getEventProcessor(T* e) = 0;
	};


	

}
}



#endif