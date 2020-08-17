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
#include "../../Framework/Timing/TimeController.h"
#include "Scoring/ScoreProcessor.h"
#include "Modifiers/DifficultyModifier.h"
#include "Modifiers/EventProcessorFilterModifier.h"
#include "Replays/ReplayRecorder.h"
#include "../Scene/Results/Result.h"



/*
 * virutal private���n�B�A�i�H���l���h�ק�����A���S����ȱ�call����
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
using namespace Framework::Timing;
using namespace Games::Rulesets::Scoring;
using namespace Games::Rulesets::Modifiers;
using namespace Games::Rulesets::Replays;
using namespace Games::Scenes::Results;




namespace Games {
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

		PassThroughInputManager* rulesetInputManager;

		

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
			// TODO: �H��n��pattern generator�\�^converter�̭��A���n�\�b�o�̡Aeffect�]�w�ᵹmap algorithm�B�z�N�n
			// ^^ �d���F�Apattern generator�u��b�o��ͦ��A�]��meteor ruleset executor�٨S����config�N�|�ͦ�converter�A�N�|�S��config��ơA�S��k��effect


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


			/*
			 * ���ثe�C���Ҧ��L����event processor�R���A�]��converter�|��Ҧ�processor�����ͥX�ӡA�ڭ̭n�ۤv�z�ۤv�n��
			 */
			EventProcessorFilter* eventProcessorFilter = createEventProcessorFilter();

			for (int i = 0; i < workingSm->GetModifiers()->GetValue()->size(); i++) {
				if (dynamic_cast<EventProcessorFilterModifier*>(workingSm->GetModifiers()->GetValue()->at(i))) {
					dynamic_cast<EventProcessorFilterModifier*>(workingSm->GetModifiers()->GetValue()->at(i))
						->ApplyToEventProcessorFilter(eventProcessorFilter);
				}
			}
			AddChild(eventProcessorFilter);
			
			/*
			 * �p�����ɭԥΨӽվ����
			 */
			for (int i = 0; i < workingSm->GetModifiers()->GetValue()->size(); i++) {
				if (dynamic_cast<DifficultyModifier*>(workingSm->GetModifiers()->GetValue()->at(i))) {
					dynamic_cast<DifficultyModifier*>(workingSm->GetModifiers()->GetValue()->at(i))
						->ApplyToDifficulty(workingSm->GetSm()->GetSmInfo()->difficuty);
				}
			}

			/*
			 * ��Ҧ��C���������סB�]�w���A�Ҧp����︨�U�t�סA���o���зǡB��ﭵ��
			 */
			for (int i = 0; i < sm->GetEvents()->size(); i++) {
				sm->GetEvents()->at(i)->ApplyDefaultValues(workingSm->GetSm()->GetSmInfo()->difficuty);
			}



			// Add mods, should always be the last thing applied to give full control to mods
			// applyMods(mods);

			LOG(LogLevel::Info) << "RulesetExecutor::load : creating playfield ...";
			playfield = createPlayfield();

			// �إߤ@�ӱ����W����input�A�ഫ���o��ruleset��input��inputmanager
			AddChild(rulesetInputManager);

			// �o��|��map algoŪ�i�hplayfield�̭��A�o��ƭn�O�o�g
			rulesetInputManager->AddChild(playfield);

			// ��Event�নEvent processor�\�i�hplayfield��
			LOG(LogLevel::Info) << "RulesetExecutor::load : loading events into playfield ...";
			playfieldLoad();

			return 0;
		}

		//int applyMods(vector<mod*>* m);

		virtual SmConverter* createSmConverter(PatternGenerator* pg) = 0;

		virtual SmPostprocessor* createSmPostprocessor() = 0;

		virtual EventProcessorFilter* createEventProcessorFilter() {
			return new EventProcessorFilter();
		}

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

		//virtual ~RulesetExecutor() = default;
		~RulesetExecutor() {
			delete sm;
			sm = nullptr;
		}

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

		virtual SpeedAdjuster* CreateSpeedAdjuster() = 0;

		virtual ScoreProcessor* CreateScoreProcessor() = 0;

		virtual ReplayRecorder* CreateReplayRecorder() = 0;

		virtual Result* CreateResult(Score* s) = 0;


		template<class _Type>
		int AddOnJudgement(_Type* callableObject, function<int(Judgement*)> callback, string name = "HandleJudgement") {


			onJudgement.Add(callableObject, callback, name);

			return 0;
		}

		Playfield* GetPlayfield() {
			return playfield;
		}

		vector<T*>* GetEvents() {
			return sm->GetEvents();
		}


	protected:

		Ruleset* ruleset;

		Sm<T>* sm = nullptr;

		WorkingSm* workingSm = nullptr;

		//vector<mod_t*> mods;

		Playfield* playfield = nullptr;

		ActionList<int, Judgement*> onJudgement;

		virtual Playfield* createPlayfield() = 0;

		/// <summary>
		/// jobs:
		/// 1. �إ߻Pevent������processor
		/// 2. �p�Gprocessor�Oeffect mapper�A�N��map algo�[�i�h
		/// </summary>
		virtual EventProcessor<T>* getEventProcessor(T* e) = 0;

		/// <summary>
		/// jobs:
		/// 1. find the objects in sm?
		/// 2. add them to playfield?
		/// </summary>
		virtual int playfieldLoad() {

			LOG(LogLevel::Fine) << "RulesetExecutor::playfieldLoad : loading [" << sm->GetEvents()->size() << "] events into playfield ... ";

			// ��Event�নEvent processor�\�i�hplayfield��
			for (int i = 0; i < sm->GetEvents()->size(); i++) {
				LOG(LogLevel::Finer) << "RulesetExecutor::playfieldLoad : getting [" << sm->GetEvents()->at(i)->GetStartTime() << "] " << sm->GetEvents()->at(i)->GetTypeName() << " processor into playfield " << playfield << " ... ";
				EventProcessor<Event>* ep = getEventProcessor(sm->GetEvents()->at(i));

				if (dynamic_cast<HitObject*>(ep)) {
					dynamic_cast<HitObject*>(ep)->AddOnJudgement(ep, [=](HitObject* h, Judgement* j) {

						playfield->OnJudgement(h, j);
						onJudgement.Trigger(j); // score processor -> on judgement

						return 0;
					}, "RulesetExecutor::HandleJudgement");
				}
				playfield->Add(ep);
			}

			return 0;
		}

	};


	

}
}



#endif