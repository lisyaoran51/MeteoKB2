#ifndef RULESET_H
#define RULESET_H


#include "RulesetInfo.h"
#include "RulesetExecutor.h"
#include "../Sheetmusic/WorkingSheetmusic.h"
#include "../../Framework/Input/KeyBindings/KeyBinding.h"
#include "../../Instruments/Input/PitchBindingSet.h"


using namespace Games::Sheetmusics;
using namespace Framework::Input::KeyBindings;
using namespace Instruments::Input;

namespace Games {
namespace Rulesets {

	class RulesetInfo;
	
	/// <summary>
	/// a game rule that designs how the game plays
	///	</summary>
	class Ruleset: public MtoObject
	{
		/// <summary>
		/// 判斷這個物建有沒有被建構，建構後才可使用
		///	</summary>
		bool constructed;

	public:

		Ruleset();

		Ruleset(RulesetInfo* r);

		virtual int LazyConstruct(RulesetInfo* r);

		~Ruleset();

		virtual RulesetExecutor<Event>* CreateRulesetExecutor(WorkingSm* sm) = 0;

		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0) = 0;

		virtual PitchBindingSet* GetDefaultPitchBindingSet(int variant = 0) = 0;

		RulesetInfo* GetRulesetInfo();

	protected:

		RulesetInfo* rulesetInfo;

	};

}}



#endif