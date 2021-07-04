#include"MeteorRuleset.h"

#include "MeteorRulesetExecutor.h"
#include "../Input/MeteorInputManager.h"
#include "../Input/MeteorPitchBindingSet.h"


using namespace Meteor::Rulesets;
using namespace Meteor::Input;


MeteorRuleset::MeteorRuleset() : RegisterType("MeteorRuleset")
{
	constructed = false;
}

MeteorRuleset::MeteorRuleset(RulesetInfo * r): RegisterType("MeteorRuleset")
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

PitchBindingSet * MeteorRuleset::GetDefaultPitchBindingSet(int variant)
{
	MeteorPitchBindingSet* pitchBindingSet = new MeteorPitchBindingSet();
	
	/* ゼど */
#pragma region NonePitchState

	pitchBindingSet->SetPitchBinding(Pitch::C   , MeteorAction::VK24_L_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::Db  , MeteorAction::VK24_L_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::D   , MeteorAction::VK24_L_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::Eb  , MeteorAction::VK24_L_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::E   , MeteorAction::VK24_L_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::F   , MeteorAction::VK24_L_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::Gb  , MeteorAction::VK24_L_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::G   , MeteorAction::VK24_L_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::Ab  , MeteorAction::VK24_L_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::A   , MeteorAction::VK24_L_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::Bb  , MeteorAction::VK24_L_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::B   , MeteorAction::VK24_L_B1 );
	
	pitchBindingSet->SetPitchBinding(Pitch::c   , MeteorAction::VK24_L_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::db  , MeteorAction::VK24_L_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d   , MeteorAction::VK24_L_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::eb  , MeteorAction::VK24_L_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e   , MeteorAction::VK24_L_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f   , MeteorAction::VK24_L_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::gb  , MeteorAction::VK24_L_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g   , MeteorAction::VK24_L_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::ab  , MeteorAction::VK24_L_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a   , MeteorAction::VK24_L_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::bb  , MeteorAction::VK24_L_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b   , MeteorAction::VK24_L_B2 );
	
	pitchBindingSet->SetPitchBinding(Pitch::c1  , MeteorAction::VK24_R_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::d1b , MeteorAction::VK24_R_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d1  , MeteorAction::VK24_R_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::e1b , MeteorAction::VK24_R_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e1  , MeteorAction::VK24_R_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f1  , MeteorAction::VK24_R_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::g1b , MeteorAction::VK24_R_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g1  , MeteorAction::VK24_R_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::a1b , MeteorAction::VK24_R_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a1  , MeteorAction::VK24_R_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::b1b , MeteorAction::VK24_R_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b1  , MeteorAction::VK24_R_B1 );
	 
	pitchBindingSet->SetPitchBinding(Pitch::c2  , MeteorAction::VK24_R_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d2b , MeteorAction::VK24_R_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d2  , MeteorAction::VK24_R_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e2b , MeteorAction::VK24_R_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e2  , MeteorAction::VK24_R_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f2  , MeteorAction::VK24_R_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g2b , MeteorAction::VK24_R_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g2  , MeteorAction::VK24_R_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a2b , MeteorAction::VK24_R_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a2  , MeteorAction::VK24_R_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b2b , MeteorAction::VK24_R_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b2  , MeteorAction::VK24_R_B2 );


#pragma endregion

	/*  */
#pragma region LoweredPitchState

	pitchBindingSet->SwitchPitchState(MeteoPianoPitchState::Lowered);

	pitchBindingSet->SetPitchBinding(Pitch::C1  , MeteorAction::VK24_L_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::D1b , MeteorAction::VK24_L_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::D1  , MeteorAction::VK24_L_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::E1b , MeteorAction::VK24_L_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::E1  , MeteorAction::VK24_L_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::F1  , MeteorAction::VK24_L_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::G1b , MeteorAction::VK24_L_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::G1  , MeteorAction::VK24_L_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::A1b , MeteorAction::VK24_L_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::A1  , MeteorAction::VK24_L_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::B1b , MeteorAction::VK24_L_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::B1  , MeteorAction::VK24_L_B1 );

	pitchBindingSet->SetPitchBinding(Pitch::C   , MeteorAction::VK24_L_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::Db  , MeteorAction::VK24_L_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::D   , MeteorAction::VK24_L_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::Eb  , MeteorAction::VK24_L_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::E   , MeteorAction::VK24_L_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::F   , MeteorAction::VK24_L_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::Gb  , MeteorAction::VK24_L_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::G   , MeteorAction::VK24_L_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::Ab  , MeteorAction::VK24_L_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::A   , MeteorAction::VK24_L_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::Bb  , MeteorAction::VK24_L_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::B   , MeteorAction::VK24_L_B2 );

	pitchBindingSet->SetPitchBinding(Pitch::c   , MeteorAction::VK24_R_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::db  , MeteorAction::VK24_R_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d   , MeteorAction::VK24_R_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::eb  , MeteorAction::VK24_R_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e   , MeteorAction::VK24_R_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f   , MeteorAction::VK24_R_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::gb  , MeteorAction::VK24_R_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g   , MeteorAction::VK24_R_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::ab  , MeteorAction::VK24_R_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a   , MeteorAction::VK24_R_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::bb  , MeteorAction::VK24_R_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b   , MeteorAction::VK24_R_B1 );

	pitchBindingSet->SetPitchBinding(Pitch::c1  , MeteorAction::VK24_R_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d1b , MeteorAction::VK24_R_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d1  , MeteorAction::VK24_R_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e1b , MeteorAction::VK24_R_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e1  , MeteorAction::VK24_R_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f1  , MeteorAction::VK24_R_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g1b , MeteorAction::VK24_R_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g1  , MeteorAction::VK24_R_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a1b , MeteorAction::VK24_R_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a1  , MeteorAction::VK24_R_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b1b , MeteorAction::VK24_R_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b1  , MeteorAction::VK24_R_B2 );

#pragma endregion

	/* ど */
#pragma region RaisedPitchState

	pitchBindingSet->SwitchPitchState(MeteoPianoPitchState::Raised);
	 
	pitchBindingSet->SetPitchBinding(Pitch::c		, MeteorAction::VK24_L_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::db		, MeteorAction::VK24_L_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d		, MeteorAction::VK24_L_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::eb		, MeteorAction::VK24_L_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e		, MeteorAction::VK24_L_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f		, MeteorAction::VK24_L_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::gb		, MeteorAction::VK24_L_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g		, MeteorAction::VK24_L_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::ab		, MeteorAction::VK24_L_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a		, MeteorAction::VK24_L_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::bb		, MeteorAction::VK24_L_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b		, MeteorAction::VK24_L_B1 );

	pitchBindingSet->SetPitchBinding(Pitch::c1		, MeteorAction::VK24_L_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d1b		, MeteorAction::VK24_L_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d1		, MeteorAction::VK24_L_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e1b		, MeteorAction::VK24_L_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e1		, MeteorAction::VK24_L_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f1		, MeteorAction::VK24_L_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g1b		, MeteorAction::VK24_L_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g1		, MeteorAction::VK24_L_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a1b		, MeteorAction::VK24_L_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a1		, MeteorAction::VK24_L_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b1b		, MeteorAction::VK24_L_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b1		, MeteorAction::VK24_L_B2 );

	pitchBindingSet->SetPitchBinding(Pitch::c2	    , MeteorAction::VK24_R_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::d2b		, MeteorAction::VK24_R_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d2	    , MeteorAction::VK24_R_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::e2b		, MeteorAction::VK24_R_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e2	    , MeteorAction::VK24_R_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f2	    , MeteorAction::VK24_R_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::g2b		, MeteorAction::VK24_R_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g2	    , MeteorAction::VK24_R_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::a2b		, MeteorAction::VK24_R_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a2	    , MeteorAction::VK24_R_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::b2b		, MeteorAction::VK24_R_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b2	    , MeteorAction::VK24_R_B1 ); 

	pitchBindingSet->SetPitchBinding(Pitch::c3	    , MeteorAction::VK24_R_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d3b		, MeteorAction::VK24_R_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d3	    , MeteorAction::VK24_R_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e3b		, MeteorAction::VK24_R_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e3	    , MeteorAction::VK24_R_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f3	    , MeteorAction::VK24_R_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g3b		, MeteorAction::VK24_R_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g3	    , MeteorAction::VK24_R_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a3b		, MeteorAction::VK24_R_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a3	    , MeteorAction::VK24_R_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b3b		, MeteorAction::VK24_R_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b3	    , MeteorAction::VK24_R_B2 );

#pragma endregion

	pitchBindingSet->SwitchPitchState(MeteoPianoPitchState::None);

	return pitchBindingSet;
}
