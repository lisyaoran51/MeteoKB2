#include"InstantRuleset.h"

#include "InstantRulesetExecutor.h"
#include "../Input/InstantInputManager.h"

using namespace Instant::Rulesets;
using namespace Instant::Input;

InstantRuleset::InstantRuleset() : RegisterType("InstantRuleset")
{
	constructed = false;
}

InstantRuleset::InstantRuleset(RulesetInfo * r): RegisterType("InstantRuleset")
{
	rulesetInfo = r;
	constructed = true;
}

int InstantRuleset::LazyConstruct(RulesetInfo * r)
{
	Ruleset::LazyConstruct(r);
	constructed = true;
	return 0;
}

RulesetExecutor<Event>* InstantRuleset::CreateRulesetExecutor(WorkingSm * sm)
{
	InstanceCreator<MtoObject>& iCreator = InstanceCreator<MtoObject>::GetInstance();

	RulesetExecutor<Event>* rulesetExecutor = iCreator.CreateInstanceWithT<InstantRulesetExecutor>("InstantRulesetExecutor");

	// instance creator⊿快猭ミ碞р把计メ秈? 
	// ⊿快猭秨﹍メ┮碞ぇㄓlazy construct 硂妓ゑ耕
	rulesetExecutor->LazyConstruct(sm, this);

	return rulesetExecutor;
}

vector<KeyBinding*>* InstantRuleset::GetDefaultkeyBindings(int variant)
{
	vector<KeyBinding*>* bindings = new vector<KeyBinding*>();

	bindings->push_back(new KeyBinding(InputKey::None, (int)InstantAction::None));

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
