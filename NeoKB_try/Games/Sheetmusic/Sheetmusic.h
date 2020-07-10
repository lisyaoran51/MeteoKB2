#ifndef SHEETMUSIC_H
#define SHEETMUSIC_H

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>    // std::sort
#include "../../Util/TemplateConstraint.h"
#include "SheetmusicInfo.h"
#include "SheetmusicMetadata.h"
#include "../Ruleset/RulesetInfo.h"
#include "../Scheduler/Event/Event.h"



using namespace std;
using namespace Util;
using namespace Games::Rulesets;
using namespace Games::Schedulers::Events;


namespace Games {
namespace Sheetmusics {
	
	/// where T : effect
	/// 
	template <typename T>
	class Sm : TConstraint<T, Event>
	{

		SmInfo* smInfo = NULL;
		
		SmMetadata* smMetadata = NULL;
		
		vector<T*>* events = NULL;

	public:
		
		Sm(Sm<T>* sm = NULL) {
			if (sm) {
				// TODO: 應該是要附置一份，而不是直接指過去
				LOG(LogLevel::Finer) << "Sm(Sm<T>*) : Getting info ...";
				SetSmInfo(sm->GetSmInfo());
				LOG(LogLevel::Finer) << "Sm(Sm<T>*) : Getting metadata ...";
				SetSmMetadata(sm->GetSmMetadata());
				LOG(LogLevel::Finer) << "Sm(Sm<T>*) : Getting events ...";
				SetEvents(sm->GetEvents());
				LOG(LogLevel::Finer) << "Sm(Sm<T>*) : Getting ruleset info ...";
				//SetRulesetInfo(sm->GetRulesetInfo());
			}
			else {
				smInfo = new SmInfo();
				smInfo->metadata = new SmMetadata();
				smInfo->difficuty = new SmDifficulty();
				// TODO: metadata到底要存哪？info裡還是sm裡
				smMetadata = smInfo->metadata;
				SetEvents(new vector<T*>());
				// SetRulesetInfo
			}
		}

		~Sm() {
			delete smInfo;
			delete smMetadata;
			delete events;
		}

		int SetSmInfo(SmInfo* si) {
			smInfo = si;
			return 0;
		}

		SmInfo* GetSmInfo() {
			return smInfo;
		}

		int SetSmMetadata(SmMetadata* sm) {
			smMetadata = sm;
			return 0;
		}

		SmMetadata* GetSmMetadata() {
			return smMetadata;
		}

		int SetEvents(vector<T*>* e) {
			events = e;
			
			// TODO: sort
			// 這個再postprocessor有作，這邊又作一次?

			sort(e->begin(), e->end());
			return 0;
		}

		vector<T*>* GetEvents() {
			return events;
		}

		/* 待刪
		int SetRulesetInfo(RulesetInfo* r) {
			rulesetInfo = r;
			return 0;
		}

		RulesetInfo* GetRulesetInfo() {
			return rulesetInfo;
		}
		*/
	};


}}



#endif