#include"InstantRuleset.h"

#include "InstantRulesetExecutor.h"
#include "../Input/InstantInputManager.h"
#include "../Input/InstantPitchBindingSet.h"

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

PitchBindingSet * InstantRuleset::GetDefaultPitchBindingSet(int variant)
{
	InstantPitchBindingSet* pitchBindingSet = new InstantPitchBindingSet();
	
	/* ゼど */
#pragma region NonePitchState

	pitchBindingSet->SetPitchBinding(Pitch::C   , InstantAction::VK24_L_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::Db  , InstantAction::VK24_L_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::D   , InstantAction::VK24_L_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::Eb  , InstantAction::VK24_L_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::E   , InstantAction::VK24_L_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::F   , InstantAction::VK24_L_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::Gb  , InstantAction::VK24_L_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::G   , InstantAction::VK24_L_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::Ab  , InstantAction::VK24_L_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::A   , InstantAction::VK24_L_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::Bb  , InstantAction::VK24_L_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::B   , InstantAction::VK24_L_B1 );
												  
	pitchBindingSet->SetPitchBinding(Pitch::c   , InstantAction::VK24_L_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::db  , InstantAction::VK24_L_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d   , InstantAction::VK24_L_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::eb  , InstantAction::VK24_L_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e   , InstantAction::VK24_L_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f   , InstantAction::VK24_L_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::gb  , InstantAction::VK24_L_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g   , InstantAction::VK24_L_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::ab  , InstantAction::VK24_L_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a   , InstantAction::VK24_L_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::bb  , InstantAction::VK24_L_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b   , InstantAction::VK24_L_B2 );
												  
	pitchBindingSet->SetPitchBinding(Pitch::c1  , InstantAction::VK24_R_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::d1b , InstantAction::VK24_R_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d1  , InstantAction::VK24_R_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::e1b , InstantAction::VK24_R_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e1  , InstantAction::VK24_R_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f1  , InstantAction::VK24_R_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::g1b , InstantAction::VK24_R_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g1  , InstantAction::VK24_R_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::a1b , InstantAction::VK24_R_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a1  , InstantAction::VK24_R_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::b1b , InstantAction::VK24_R_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b1  , InstantAction::VK24_R_B1 );
	 											  
	pitchBindingSet->SetPitchBinding(Pitch::c2  , InstantAction::VK24_R_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d2b , InstantAction::VK24_R_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d2  , InstantAction::VK24_R_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e2b , InstantAction::VK24_R_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e2  , InstantAction::VK24_R_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f2  , InstantAction::VK24_R_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g2b , InstantAction::VK24_R_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g2  , InstantAction::VK24_R_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a2b , InstantAction::VK24_R_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a2  , InstantAction::VK24_R_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b2b , InstantAction::VK24_R_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b2  , InstantAction::VK24_R_B2 );


#pragma endregion

	/*  */
#pragma region LoweredPitchState

	pitchBindingSet->SwitchPitchState(MeteoPianoPitchState::Lowered);

	pitchBindingSet->SetPitchBinding(Pitch::C1  , InstantAction::VK24_L_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::D1b , InstantAction::VK24_L_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::D1  , InstantAction::VK24_L_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::E1b , InstantAction::VK24_L_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::E1  , InstantAction::VK24_L_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::F1  , InstantAction::VK24_L_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::G1b , InstantAction::VK24_L_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::G1  , InstantAction::VK24_L_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::A1b , InstantAction::VK24_L_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::A1  , InstantAction::VK24_L_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::B1b , InstantAction::VK24_L_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::B1  , InstantAction::VK24_L_B1 );
												  
	pitchBindingSet->SetPitchBinding(Pitch::C   , InstantAction::VK24_L_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::Db  , InstantAction::VK24_L_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::D   , InstantAction::VK24_L_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::Eb  , InstantAction::VK24_L_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::E   , InstantAction::VK24_L_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::F   , InstantAction::VK24_L_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::Gb  , InstantAction::VK24_L_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::G   , InstantAction::VK24_L_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::Ab  , InstantAction::VK24_L_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::A   , InstantAction::VK24_L_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::Bb  , InstantAction::VK24_L_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::B   , InstantAction::VK24_L_B2 );
												  
	pitchBindingSet->SetPitchBinding(Pitch::c   , InstantAction::VK24_R_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::db  , InstantAction::VK24_R_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d   , InstantAction::VK24_R_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::eb  , InstantAction::VK24_R_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e   , InstantAction::VK24_R_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f   , InstantAction::VK24_R_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::gb  , InstantAction::VK24_R_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g   , InstantAction::VK24_R_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::ab  , InstantAction::VK24_R_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a   , InstantAction::VK24_R_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::bb  , InstantAction::VK24_R_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b   , InstantAction::VK24_R_B1 );
												  
	pitchBindingSet->SetPitchBinding(Pitch::c1  , InstantAction::VK24_R_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d1b , InstantAction::VK24_R_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d1  , InstantAction::VK24_R_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e1b , InstantAction::VK24_R_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e1  , InstantAction::VK24_R_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f1  , InstantAction::VK24_R_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g1b , InstantAction::VK24_R_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g1  , InstantAction::VK24_R_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a1b , InstantAction::VK24_R_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a1  , InstantAction::VK24_R_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b1b , InstantAction::VK24_R_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b1  , InstantAction::VK24_R_B2 );

#pragma endregion

	/* ど */
#pragma region RaisedPitchState

	pitchBindingSet->SwitchPitchState(MeteoPianoPitchState::Raised);
	 
	pitchBindingSet->SetPitchBinding(Pitch::c		, InstantAction::VK24_L_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::db		, InstantAction::VK24_L_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d		, InstantAction::VK24_L_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::eb		, InstantAction::VK24_L_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e		, InstantAction::VK24_L_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f		, InstantAction::VK24_L_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::gb		, InstantAction::VK24_L_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g		, InstantAction::VK24_L_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::ab		, InstantAction::VK24_L_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a		, InstantAction::VK24_L_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::bb		, InstantAction::VK24_L_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b		, InstantAction::VK24_L_B1 );
													  
	pitchBindingSet->SetPitchBinding(Pitch::c1		, InstantAction::VK24_L_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d1b		, InstantAction::VK24_L_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d1		, InstantAction::VK24_L_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e1b		, InstantAction::VK24_L_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e1		, InstantAction::VK24_L_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f1		, InstantAction::VK24_L_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g1b		, InstantAction::VK24_L_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g1		, InstantAction::VK24_L_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a1b		, InstantAction::VK24_L_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a1		, InstantAction::VK24_L_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b1b		, InstantAction::VK24_L_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b1		, InstantAction::VK24_L_B2 );
													  
	pitchBindingSet->SetPitchBinding(Pitch::c2	    , InstantAction::VK24_R_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::d2b		, InstantAction::VK24_R_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d2	    , InstantAction::VK24_R_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::e2b		, InstantAction::VK24_R_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e2	    , InstantAction::VK24_R_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f2	    , InstantAction::VK24_R_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::g2b		, InstantAction::VK24_R_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g2	    , InstantAction::VK24_R_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::a2b		, InstantAction::VK24_R_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a2	    , InstantAction::VK24_R_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::b2b		, InstantAction::VK24_R_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b2	    , InstantAction::VK24_R_B1 ); 
													  
	pitchBindingSet->SetPitchBinding(Pitch::c3	    , InstantAction::VK24_R_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d3b		, InstantAction::VK24_R_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d3	    , InstantAction::VK24_R_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e3b		, InstantAction::VK24_R_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e3	    , InstantAction::VK24_R_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f3	    , InstantAction::VK24_R_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g3b		, InstantAction::VK24_R_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g3	    , InstantAction::VK24_R_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a3b		, InstantAction::VK24_R_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a3	    , InstantAction::VK24_R_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b3b		, InstantAction::VK24_R_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b3	    , InstantAction::VK24_R_B2 );

#pragma endregion

	pitchBindingSet->SwitchPitchState(MeteoPianoPitchState::None);

	return pitchBindingSet;
}
