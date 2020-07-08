#ifndef METEOR_RULESET_EXCUTOR_H
#define METEOR_RULESET_EXCUTOR_H

#include "../../Games/Ruleset/Ruleset.h"
#include "../../Framework/Input/PassThroughInputManager.h"
#include "../../Framework/Timing/TimeController.h"
#include "../../Games/Ruleset/Scoring/ScoreProcessor.h"
#include "../Timing/MeteorTimeController.h"


using namespace Games::Rulesets;
using namespace Games::Schedulers::Events;
using namespace Framework::Input;
using namespace Framework::Timing;
using namespace Games::Rulesets::Scoring;
using namespace Meteor::Timing;




namespace Meteor {
namespace Rulesets {

	class MeteorRulesetExecutor: public RulesetExecutor<Event>{

		/// <summary>
		/// �P�_�o�Ӫ��ئ��S���Q�غc�A�غc��~�i�ϥ�
		///	</summary>
		bool constructed;

		///<summary>
		/// �o�Ӹհ�����Ϊ�
		/// �bget event processor�ɡA��o��event�n�Ψ쪺effect mapper�\�i�h???
		/// **�γo�Ӥ��Ӧn�A�į�|�ܮt�A���Ӫ����ΧP�_���A���n�dtable
		/// **�dtable���n�B�O�i�H��executor�Mevent processor���}�A���C���X
		///</summary>
		map<string, string> eventProcessorTable;

		virtual SmConverter* createSmConverter(PatternGenerator* pg);

		virtual SmPostprocessor* createSmPostprocessor();

		int load();

		int load(MeteorTimeController* t);

	public:

		MeteorRulesetExecutor();

		virtual ~MeteorRulesetExecutor() = default;

		virtual int LazyConstruct(WorkingSm* w, Ruleset* r);

		virtual PassThroughInputManager* CreateInputManager();

		virtual TimeController* CreateTimeController();
		
		virtual SpeedAdjuster* CreateSpeedAdjuster();

		virtual ScoreProcessor* CreateScoreProcessor();

		virtual int Elapse(MTO_FLOAT elapsedTime);

	protected:

		Playfield* createPlayfield();

		virtual EventProcessor<Event>* getEventProcessor(Event* e);

		string GetProcessorType(string eventType);

		virtual int playfieldLoad();

	};

}
}







#endif