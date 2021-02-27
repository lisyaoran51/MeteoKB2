#include"RecordRuleset.h"

#include "RecordRulesetExecutor.h"
#include "../Input/RecordInputManager.h"

using namespace Record::Rulesets;
using namespace Record::Input;

RecordRuleset::RecordRuleset() : RegisterType("RecordRuleset")
{
	constructed = false;
}

RecordRuleset::RecordRuleset(RulesetInfo * r): RegisterType("RecordRuleset")
{
	rulesetInfo = r;
	constructed = true;
}

int RecordRuleset::LazyConstruct(RulesetInfo * r)
{
	Ruleset::LazyConstruct(r);
	constructed = true;
	return 0;
}

RulesetExecutor<Event>* RecordRuleset::CreateRulesetExecutor(WorkingSm * sm)
{
	InstanceCreator<MtoObject>& iCreator = InstanceCreator<MtoObject>::GetInstance();

	RulesetExecutor<Event>* rulesetExecutor = iCreator.CreateInstanceWithT<RecordRulesetExecutor>("RecordRulesetExecutor");

	// instance creator⊿快猭ミ碞р把计メ秈? 
	// ⊿快猭秨﹍メ┮碞ぇㄓlazy construct 硂妓ゑ耕
	rulesetExecutor->LazyConstruct(sm, this);

	return rulesetExecutor;
}

vector<KeyBinding*>* RecordRuleset::GetDefaultkeyBindings(int variant)
{
	vector<KeyBinding*>* bindings = new vector<KeyBinding*>();

	bindings->push_back(new KeyBinding(InputKey::None, (int)RecordAction::None));

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

PitchBindingSet * RecordRuleset::GetDefaultPitchBindingSet(int variant)
{
	LOG(LogLevel::Error) << "int RecordRuleset::GetDefaultPitchBindingSet() : not implemented.";
	return nullptr;
}
