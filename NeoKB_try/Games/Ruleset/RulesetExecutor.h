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
#include "../../Framework/Input/PassThroughInputManager.h"
#include "../Scene/Play/TimeController.h"



/*
 * virutal private的好處，可以讓子類去修改父類，但又不能值接call父類
 * https://stackoverflow.com/questions/2170688/private-virtual-method-in-c
 */



using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events;
using namespace Games::Sheetmusics;
using namespace Games::Sheetmusics::Patterns;
using namespace Framework::Configurations;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Games::Rulesets;
using namespace Framework::Allocation::Hierachal;
using namespace Games::Scenes::Play;
using namespace Framework::Input;



namespace Games {
namespace Rulesets {
	
	/// <summary>
	/// a game rule that designs how the game plays
	///	</summary>
	template<class T>
	class RulesetExecutor: private TConstraint<T, Event>, public Container
	{

		/// <summary>
		/// 判斷這個物建有沒有被建構，建構後才可使用
		///	</summary>
		bool constructed;

		PassThroughInputManager* rulesetInputManager;

		/// <summary>
		/// jobs:
		/// 1. find the objects in sm?
		/// 2. add them to playfield?
		/// </summary>
		int playfieldLoad() {

			LOG(LogLevel::Fine) << "RulesetExecutor::playfieldLoad : loading [" << sm->GetEvents()->size() << "] events into playfield ... ";

			// 把Event轉成Event processor擺進去playfield裡
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
		/// load入遊戲狀態
		/// </summary>
		int load(FrameworkConfigManager* f) {
			// 取pattern generator的名字
			string pgName;

			if (!f->Get(FrameworkSetting::PatternGenerator, &pgName))
				throw runtime_error("int RulesetExecutor<T>::load(FrameworkConfigManager*) : PatternGenerator not found in Setting.");

			// 利用pattern generator的名字建立pattern generator
			LOG(LogLevel::Info) << "RulesetExecutor::load : creating [" << pgName << "] ...";
			InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
			PatternGenerator* pg = iCreator.CreateInstanceWithT<PatternGenerator>(pgName);
			AddChild(pg);

			// 要把converter和postprocessor擺到load()裡，因為pattern Generator是擺在cache裡的
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

			// 建立一個接收上面的input，轉換成這個ruleset的input的inputmanager
			AddChild(rulesetInputManager);

			// 這邊會把map algo讀進去playfield裡面，這件事要記得寫
			rulesetInputManager->AddChild(playfield);

			// 把Event轉成Event processor擺進去playfield裡
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
			// 註冊private load (c++才需要)
			registerLoad(bind(static_cast<int(RulesetExecutor<T>::*)(void)>(&RulesetExecutor<T>::load), this));
			constructed = false;
		}

		virtual ~RulesetExecutor() = default;

		virtual int LazyConstruct(WorkingSm* w, Ruleset* r) {

			workingSm = w;

			ruleset = r;
			//mods = w->get_mods();

			constructed = true;

			rulesetInputManager = CreateInputManager();

			return 0;
		}

		virtual PassThroughInputManager* CreateInputManager() = 0;

		virtual TimeController* CreateTimeController() = 0;


	protected:

		Ruleset* ruleset;

		Sm<T>* sm = nullptr;

		WorkingSm* workingSm = nullptr;

		//vector<mod_t*> mods;

		Playfield* playfield = nullptr;

		vector<void*> on_judgement;

		virtual Playfield* createPlayfield() = 0;

		/// <summary>
		/// jobs:
		/// 1. 建立與event對應的processor
		/// 2. 如果processor是effect mapper，就把map algo加進去
		/// </summary>
		virtual EventProcessor<T>* getEventProcessor(T* e) = 0;
	};


	

}
}



#endif