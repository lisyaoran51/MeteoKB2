#include"MeteorRuleset.h"

#include "MeteorRulesetExecutor.h"
#include "../Input/MeteorInputManager.h"

using namespace Meteor::Rulesets;
using namespace Meteor::Input;

MeteorRuleset::MeteorRuleset() : RegisterType("Ruleset")
{
	constructed = false;
}

MeteorRuleset::MeteorRuleset(RulesetInfo * r): RegisterType("Ruleset")
{
	rulesetInfo = r;
	constructed = true;
}

int MeteorRuleset::LazyConstruct(RulesetInfo * r)
{
	Ruleset::LazyConstruct(r);
	constructed = true;
	return 0;
}

RulesetExecutor<Event>* MeteorRuleset::CreateRulesetExecutor(WorkingSm * sm)
{
	InstanceCreator<MtoObject>& iCreator = InstanceCreator<MtoObject>::GetInstance();

	RulesetExecutor<Event>* rulesetExecutor = iCreator.CreateInstanceWithT<MeteorRulesetExecutor>("MeteorRulesetExecutor");

	// 因為instance creator沒辦法在建立的時候就把參數丟進去? 
	// 沒辦法一開始丟，所以就之後再來lazy construct ，這樣比較安全
	rulesetExecutor->LazyConstruct(sm);

	return rulesetExecutor;
}

vector<KeyBinding*>* MeteorRuleset::GetDefaultkeyBindings(int variant)
{
	vector<KeyBinding*>* bindings = new vector<KeyBinding*>();

	bindings->push_back(new KeyBinding(InputKey::None, (int)MeteorAction::None));

	/* key */
	for (int i = (int)InputKey::VK27_A1; i <= (int)InputKey::VK37_C4; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* pedal */
	for (int i = (int)InputKey::SustainPedal; i <= (int)InputKey::ExpressionPedal; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* Panel */
	for (int i = (int)InputKey::RaiseOctave; i <= (int)InputKey::Bluetooth; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* Plugin */
	for (int i = (int)InputKey::BluetoothPlugin; i <= (int)InputKey::ExpressionPedalPlugin; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	return bindings;
}
