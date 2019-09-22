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

	// �]��instance creator�S��k�b�إߪ��ɭԴN��Ѽƥ�i�h? 
	// �S��k�@�}�l��A�ҥH�N����A��lazy construct �A�o�ˤ���w��
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

	return bindings;
}
