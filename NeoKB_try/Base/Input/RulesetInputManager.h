#ifndef RULESET_INPUT_MANAGER_H
#define RULESET_INPUT_MANAGER_H

#include "../../Framework/Input/KeyBindings/KeyBindingInputManager.h"
#include "../Ruleset/RulesetInfo.h"

using namespace Framework::Input::KeyBindings;
using namespace Base::Rulesets;


namespace Base {
namespace Input {

	/// <summary>
	/// �o�Өƫإߤ@�Ӥ�ʱ��clock�Mreplay input handler�A�b�������ɭԥΡA�p�G���O�������ܡA�N�S���@��
	/// �i�H�����κޥL�A����A�g
	/// �t�~�L������DatabasedKeyBindingInputManager���B�~���\��A�i�H����ruleset��keybinding�A������o�ӥ\���\��o��class��
	/// </summary>
	template<typename T>
	class RulesetInputManager : public KeyBindingInputManager<T> {

		RulesetInfo* rulesetInfo;

		int variant;

	public:

		RulesetInputManager(RulesetInfo* rInfo, int v = 0) : RegisterType("RulesetInputManager") {
			rulesetInfo = rInfo;
			variant = v;
		}

		virtual vector<KeyBinding*>* GetDefaultkeyBindings() {

			Ruleset* ruleset = rulesetInfo->CreateRuleset();
			vector<KeyBinding*>* bindings = ruleset->GetDefaultkeyBindings();
			delete ruleset;

			return bindings;
		}

	protected:

		/// <summary>
		/// �o�Ӫ��\��u�O�AGetDefaultkeyBindings()�u����S��variant��key binding�A���O�o��i�H�[�Jvariant
		/// �bosu�̭��A�ն]��database�̭���쨺��ruleset ID��keybinding�A�M��A���key binding�A�o�ˤlkey binding�N�i�H�H�N�վ�
		/// ���ڭ����Ӥ��ݭn�A�ҥH�N������database���\��
		/// </summary>
		virtual int reloadMappings() {

			Ruleset* ruleset = rulesetInfo->CreateRuleset();
			vector<KeyBinding*>* bindings = ruleset->GetDefaultkeyBindings(variant);
			delete ruleset;

			keyBindings.reserve(bindings->size());
			keyBindings.insert(keyBindings.end(), bindings->begin(), bindings->end());
			delete bindings;

			return 0;
		}
	};


}}





#endif