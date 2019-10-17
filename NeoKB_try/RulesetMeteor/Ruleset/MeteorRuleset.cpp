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

	// instance creator⊿快猭ミ碞р把计メ秈? 
	// ⊿快猭秨﹍メ┮碞ぇㄓlazy construct 硂妓ゑ耕
	rulesetExecutor->LazyConstruct(sm, this);

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

	/* General Button */
	for (int i = (int)InputKey::Power; i <= (int)InputKey::Sensitivity; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* SpecialButton */
	for (int i = (int)InputKey::RaiseOctave; i <= (int)InputKey::Pause; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* Knob */
	for (int i = (int)InputKey::SectionKnob; i <= (int)InputKey::SpeedKnob; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* Slider */
	for (int i = (int)InputKey::PianoVolumeSlider; i <= (int)InputKey::MusicVolumeSlider; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* Bluetooth */
	bindings->push_back(new KeyBinding(InputKey::Bluetooth, (int)InputKey::Bluetooth));

	/* Plugin */
	for (int i = (int)InputKey::BluetoothPlugin; i <= (int)InputKey::ExpressionPedalPlugin; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	return bindings;
}
