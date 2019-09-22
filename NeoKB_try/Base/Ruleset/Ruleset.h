#ifndef RULESET_H
#define RULESET_H


#include"RulesetInfo.h"
#include"RulesetExecutor.h"
#include "../Sheetmusic/WorkingSheetmusic.h"
#include "../../Framework/Input/KeyBindings/KeyBinding.h"


using namespace Base::Sheetmusics;
using namespace Framework::Input::KeyBindings;

namespace Base {
namespace Rulesets {

	class RulesetInfo;
	
	/// <summary>
	/// a game rule that designs how the game plays
	///	</summary>
	class Ruleset: public MtoObject
	{
		/// <summary>
		/// �P�_�o�Ӫ��ئ��S���Q�غc�A�غc��~�i�ϥ�
		///	</summary>
		bool constructed;

	public:

		Ruleset();

		Ruleset(RulesetInfo* r);

		virtual int LazyConstruct(RulesetInfo* r);

		~Ruleset();

		virtual RulesetExecutor<Event>* CreateRulesetExecutor(WorkingSm* sm) = 0;

		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0) = 0;

	protected:

		RulesetInfo* rulesetInfo;

	};

}}



#endif