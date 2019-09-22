#ifndef RULESET_INPUT_MANAGER_H
#define RULESET_INPUT_MANAGER_H

#include "../../Framework/Input/KeyBindings/KeyBindingInputManager.h"
#include "../Ruleset/RulesetInfo.h"

using namespace Framework::Input::KeyBindings;
using namespace Base::Rulesets;


namespace Base {
namespace Input {

	/// <summary>
	/// 這個事建立一個手動控制的clock和replay input handler，在重播的時候用，如果不是重播的話，就沒有作用
	/// 可以先不用管他，之後再寫
	/// 另外他的父類DatabasedKeyBindingInputManager有額外的功能，可以拿到ruleset的keybinding，直接把這個功能擺到這個class裡
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
		/// 這個的功能只是，GetDefaultkeyBindings()只能抓到沒有variant的key binding，但是這邊可以加入variant
		/// 在osu裡面，試跑到database裡面找到那個ruleset ID的keybinding，然後再抓到key binding，這樣子key binding就可以隨意調整
		/// 但我們應該不需要，所以就先不做database的功能
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