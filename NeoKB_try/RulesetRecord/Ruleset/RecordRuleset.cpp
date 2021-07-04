#include"RecordRuleset.h"

#include "RecordRulesetExecutor.h"
#include "../Input/RecordInputManager.h"
#include "../Input/RecordPitchBindingSet.h"


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
	RecordPitchBindingSet* pitchBindingSet = new RecordPitchBindingSet();
	
	/* ゼど */
#pragma region NonePitchState

	pitchBindingSet->SetPitchBinding(Pitch::C   , RecordAction::VK24_L_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::Db  , RecordAction::VK24_L_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::D   , RecordAction::VK24_L_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::Eb  , RecordAction::VK24_L_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::E   , RecordAction::VK24_L_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::F   , RecordAction::VK24_L_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::Gb  , RecordAction::VK24_L_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::G   , RecordAction::VK24_L_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::Ab  , RecordAction::VK24_L_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::A   , RecordAction::VK24_L_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::Bb  , RecordAction::VK24_L_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::B   , RecordAction::VK24_L_B1 );
												  
	pitchBindingSet->SetPitchBinding(Pitch::c   , RecordAction::VK24_L_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::db  , RecordAction::VK24_L_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d   , RecordAction::VK24_L_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::eb  , RecordAction::VK24_L_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e   , RecordAction::VK24_L_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f   , RecordAction::VK24_L_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::gb  , RecordAction::VK24_L_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g   , RecordAction::VK24_L_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::ab  , RecordAction::VK24_L_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a   , RecordAction::VK24_L_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::bb  , RecordAction::VK24_L_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b   , RecordAction::VK24_L_B2 );
												  
	pitchBindingSet->SetPitchBinding(Pitch::c1  , RecordAction::VK24_R_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::d1b , RecordAction::VK24_R_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d1  , RecordAction::VK24_R_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::e1b , RecordAction::VK24_R_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e1  , RecordAction::VK24_R_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f1  , RecordAction::VK24_R_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::g1b , RecordAction::VK24_R_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g1  , RecordAction::VK24_R_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::a1b , RecordAction::VK24_R_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a1  , RecordAction::VK24_R_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::b1b , RecordAction::VK24_R_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b1  , RecordAction::VK24_R_B1 );
	 											  
	pitchBindingSet->SetPitchBinding(Pitch::c2  , RecordAction::VK24_R_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d2b , RecordAction::VK24_R_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d2  , RecordAction::VK24_R_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e2b , RecordAction::VK24_R_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e2  , RecordAction::VK24_R_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f2  , RecordAction::VK24_R_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g2b , RecordAction::VK24_R_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g2  , RecordAction::VK24_R_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a2b , RecordAction::VK24_R_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a2  , RecordAction::VK24_R_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b2b , RecordAction::VK24_R_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b2  , RecordAction::VK24_R_B2 );


#pragma endregion

	/*  */
#pragma region LoweredPitchState

	pitchBindingSet->SwitchPitchState(MeteoPianoPitchState::Lowered);

	pitchBindingSet->SetPitchBinding(Pitch::C1  , RecordAction::VK24_L_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::D1b , RecordAction::VK24_L_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::D1  , RecordAction::VK24_L_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::E1b , RecordAction::VK24_L_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::E1  , RecordAction::VK24_L_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::F1  , RecordAction::VK24_L_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::G1b , RecordAction::VK24_L_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::G1  , RecordAction::VK24_L_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::A1b , RecordAction::VK24_L_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::A1  , RecordAction::VK24_L_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::B1b , RecordAction::VK24_L_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::B1  , RecordAction::VK24_L_B1 );
												  
	pitchBindingSet->SetPitchBinding(Pitch::C   , RecordAction::VK24_L_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::Db  , RecordAction::VK24_L_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::D   , RecordAction::VK24_L_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::Eb  , RecordAction::VK24_L_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::E   , RecordAction::VK24_L_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::F   , RecordAction::VK24_L_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::Gb  , RecordAction::VK24_L_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::G   , RecordAction::VK24_L_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::Ab  , RecordAction::VK24_L_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::A   , RecordAction::VK24_L_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::Bb  , RecordAction::VK24_L_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::B   , RecordAction::VK24_L_B2 );
												  
	pitchBindingSet->SetPitchBinding(Pitch::c   , RecordAction::VK24_R_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::db  , RecordAction::VK24_R_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d   , RecordAction::VK24_R_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::eb  , RecordAction::VK24_R_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e   , RecordAction::VK24_R_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f   , RecordAction::VK24_R_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::gb  , RecordAction::VK24_R_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g   , RecordAction::VK24_R_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::ab  , RecordAction::VK24_R_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a   , RecordAction::VK24_R_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::bb  , RecordAction::VK24_R_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b   , RecordAction::VK24_R_B1 );
												  
	pitchBindingSet->SetPitchBinding(Pitch::c1  , RecordAction::VK24_R_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d1b , RecordAction::VK24_R_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d1  , RecordAction::VK24_R_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e1b , RecordAction::VK24_R_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e1  , RecordAction::VK24_R_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f1  , RecordAction::VK24_R_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g1b , RecordAction::VK24_R_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g1  , RecordAction::VK24_R_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a1b , RecordAction::VK24_R_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a1  , RecordAction::VK24_R_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b1b , RecordAction::VK24_R_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b1  , RecordAction::VK24_R_B2 );

#pragma endregion

	/* ど */
#pragma region RaisedPitchState

	pitchBindingSet->SwitchPitchState(MeteoPianoPitchState::Raised);
	 
	pitchBindingSet->SetPitchBinding(Pitch::c		, RecordAction::VK24_L_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::db		, RecordAction::VK24_L_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d		, RecordAction::VK24_L_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::eb		, RecordAction::VK24_L_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e		, RecordAction::VK24_L_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f		, RecordAction::VK24_L_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::gb		, RecordAction::VK24_L_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g		, RecordAction::VK24_L_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::ab		, RecordAction::VK24_L_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a		, RecordAction::VK24_L_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::bb		, RecordAction::VK24_L_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b		, RecordAction::VK24_L_B1 );
													  
	pitchBindingSet->SetPitchBinding(Pitch::c1		, RecordAction::VK24_L_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d1b		, RecordAction::VK24_L_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d1		, RecordAction::VK24_L_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e1b		, RecordAction::VK24_L_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e1		, RecordAction::VK24_L_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f1		, RecordAction::VK24_L_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g1b		, RecordAction::VK24_L_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g1		, RecordAction::VK24_L_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a1b		, RecordAction::VK24_L_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a1		, RecordAction::VK24_L_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b1b		, RecordAction::VK24_L_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b1		, RecordAction::VK24_L_B2 );
													  
	pitchBindingSet->SetPitchBinding(Pitch::c2	    , RecordAction::VK24_R_C1 );
	pitchBindingSet->SetPitchBinding(Pitch::d2b		, RecordAction::VK24_R_bD1);
	pitchBindingSet->SetPitchBinding(Pitch::d2	    , RecordAction::VK24_R_D1 );
	pitchBindingSet->SetPitchBinding(Pitch::e2b		, RecordAction::VK24_R_bE1);
	pitchBindingSet->SetPitchBinding(Pitch::e2	    , RecordAction::VK24_R_E1 );
	pitchBindingSet->SetPitchBinding(Pitch::f2	    , RecordAction::VK24_R_F1 );
	pitchBindingSet->SetPitchBinding(Pitch::g2b		, RecordAction::VK24_R_bG1);
	pitchBindingSet->SetPitchBinding(Pitch::g2	    , RecordAction::VK24_R_G1 );
	pitchBindingSet->SetPitchBinding(Pitch::a2b		, RecordAction::VK24_R_bA1);
	pitchBindingSet->SetPitchBinding(Pitch::a2	    , RecordAction::VK24_R_A1 );
	pitchBindingSet->SetPitchBinding(Pitch::b2b		, RecordAction::VK24_R_bB1);
	pitchBindingSet->SetPitchBinding(Pitch::b2	    , RecordAction::VK24_R_B1 ); 
													  
	pitchBindingSet->SetPitchBinding(Pitch::c3	    , RecordAction::VK24_R_C2 );
	pitchBindingSet->SetPitchBinding(Pitch::d3b		, RecordAction::VK24_R_bD2);
	pitchBindingSet->SetPitchBinding(Pitch::d3	    , RecordAction::VK24_R_D2 );
	pitchBindingSet->SetPitchBinding(Pitch::e3b		, RecordAction::VK24_R_bE2);
	pitchBindingSet->SetPitchBinding(Pitch::e3	    , RecordAction::VK24_R_E2 );
	pitchBindingSet->SetPitchBinding(Pitch::f3	    , RecordAction::VK24_R_F2 );
	pitchBindingSet->SetPitchBinding(Pitch::g3b		, RecordAction::VK24_R_bG2);
	pitchBindingSet->SetPitchBinding(Pitch::g3	    , RecordAction::VK24_R_G2 );
	pitchBindingSet->SetPitchBinding(Pitch::a3b		, RecordAction::VK24_R_bA2);
	pitchBindingSet->SetPitchBinding(Pitch::a3	    , RecordAction::VK24_R_A2 );
	pitchBindingSet->SetPitchBinding(Pitch::b3b		, RecordAction::VK24_R_bB2);
	pitchBindingSet->SetPitchBinding(Pitch::b3	    , RecordAction::VK24_R_B2 );

#pragma endregion

	pitchBindingSet->SwitchPitchState(MeteoPianoPitchState::None);

	return pitchBindingSet;
}
