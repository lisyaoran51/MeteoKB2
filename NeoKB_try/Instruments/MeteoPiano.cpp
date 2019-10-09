#include "MeteoPiano.h"

#include "Input\PianoAction.h"
#include "Pitch.h"


using namespace Instruments;
using namespace Instruments::Input;




int MeteoPiano::OnKnobTurn(pair<PianoAction, int> action)
{
	return 0;
}

int MeteoPiano::OnSlide(pair<PianoAction, int> action)
{
	return 0;
}

map<PianoAction, SampleChannel*>* MeteoPiano::getSamples()
{
	switch (state) {
	
	case PianoPitchState::None:
		return Instrument::getSamples();
		break;
	
	case PianoPitchState::Lowered:
		return &loweredSamples;
		break;

	case PianoPitchState::Raised:
		return &raisedSamples;
		break;
	}
	return nullptr;
}

int MeteoPiano::loadAndMapSamples()
{
	/* 未升降 */
#pragma region NonePitchState

	samples.insert(PianoAction::VK24_L_C1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::C  )));
	samples.insert(PianoAction::VK24_L_bD1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Db )));
	samples.insert(PianoAction::VK24_L_D1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::D  )));
	samples.insert(PianoAction::VK24_L_bE1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Eb )));
	samples.insert(PianoAction::VK24_L_E1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::E  )));
	samples.insert(PianoAction::VK24_L_F1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::F  )));
	samples.insert(PianoAction::VK24_L_bG1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Gb )));
	samples.insert(PianoAction::VK24_L_G1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::G  )));
	samples.insert(PianoAction::VK24_L_bA1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Ab )));
	samples.insert(PianoAction::VK24_L_A1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::A  )));
	samples.insert(PianoAction::VK24_L_bB1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Bb )));
	samples.insert(PianoAction::VK24_L_B1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::B  )));
	
	samples.insert(PianoAction::VK24_L_C2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::c  )));
	samples.insert(PianoAction::VK24_L_bD2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::db )));
	samples.insert(PianoAction::VK24_L_D2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d  )));
	samples.insert(PianoAction::VK24_L_bE2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::eb )));
	samples.insert(PianoAction::VK24_L_E2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e  )));
	samples.insert(PianoAction::VK24_L_F2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::f  )));
	samples.insert(PianoAction::VK24_L_bG2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::gb )));
	samples.insert(PianoAction::VK24_L_G2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g  )));
	samples.insert(PianoAction::VK24_L_bA2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::ab )));
	samples.insert(PianoAction::VK24_L_A2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a  )));
	samples.insert(PianoAction::VK24_L_bB2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::bb )));
	samples.insert(PianoAction::VK24_L_B2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b  )));
	
	samples.insert(PianoAction::VK24_R_C1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::c1  )));
	samples.insert(PianoAction::VK24_R_bD1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d1b )));
	samples.insert(PianoAction::VK24_R_D1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d1  )));
	samples.insert(PianoAction::VK24_R_bE1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e1b )));
	samples.insert(PianoAction::VK24_R_E1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e1  )));
	samples.insert(PianoAction::VK24_R_F1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::f1  )));
	samples.insert(PianoAction::VK24_R_bG1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g1b )));
	samples.insert(PianoAction::VK24_R_G1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g1  )));
	samples.insert(PianoAction::VK24_R_bA1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a1b )));
	samples.insert(PianoAction::VK24_R_A1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a1  )));
	samples.insert(PianoAction::VK24_R_bB1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b1b )));
	samples.insert(PianoAction::VK24_R_B1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b1  )));
	
	samples.insert(PianoAction::VK24_R_C2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::c2  )));
	samples.insert(PianoAction::VK24_R_bD2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d2b )));
	samples.insert(PianoAction::VK24_R_D2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d2  )));
	samples.insert(PianoAction::VK24_R_bE2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e2b )));
	samples.insert(PianoAction::VK24_R_E2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e2  )));
	samples.insert(PianoAction::VK24_R_F2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::f2  )));
	samples.insert(PianoAction::VK24_R_bG2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g2b )));
	samples.insert(PianoAction::VK24_R_G2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g2  )));
	samples.insert(PianoAction::VK24_R_bA2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a2b )));
	samples.insert(PianoAction::VK24_R_A2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a2  )));
	samples.insert(PianoAction::VK24_R_bB2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b2b )));
	samples.insert(PianoAction::VK24_R_B2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b2  )));

#pragma endregion

	/* 降八度 */
#pragma region LoweredPitchState

	loweredSamples.insert(PianoAction::VK24_L_C1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::C1  )));
	loweredSamples.insert(PianoAction::VK24_L_bD1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::D1b )));
	loweredSamples.insert(PianoAction::VK24_L_D1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::D1  )));
	loweredSamples.insert(PianoAction::VK24_L_bE1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::E1b )));
	loweredSamples.insert(PianoAction::VK24_L_E1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::E1  )));
	loweredSamples.insert(PianoAction::VK24_L_F1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::F1  )));
	loweredSamples.insert(PianoAction::VK24_L_bG1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::G1b )));
	loweredSamples.insert(PianoAction::VK24_L_G1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::G1  )));
	loweredSamples.insert(PianoAction::VK24_L_bA1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::A1b )));
	loweredSamples.insert(PianoAction::VK24_L_A1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::A1  )));
	loweredSamples.insert(PianoAction::VK24_L_bB1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::B1b )));
	loweredSamples.insert(PianoAction::VK24_L_B1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::B1  )));
	
	loweredSamples.insert(PianoAction::VK24_L_C2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::C  )));
	loweredSamples.insert(PianoAction::VK24_L_bD2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Db )));
	loweredSamples.insert(PianoAction::VK24_L_D2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::D  )));
	loweredSamples.insert(PianoAction::VK24_L_bE2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Eb )));
	loweredSamples.insert(PianoAction::VK24_L_E2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::E  )));
	loweredSamples.insert(PianoAction::VK24_L_F2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::F  )));
	loweredSamples.insert(PianoAction::VK24_L_bG2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Gb )));
	loweredSamples.insert(PianoAction::VK24_L_G2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::G  )));
	loweredSamples.insert(PianoAction::VK24_L_bA2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Ab )));
	loweredSamples.insert(PianoAction::VK24_L_A2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::A  )));
	loweredSamples.insert(PianoAction::VK24_L_bB2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Bb )));
	loweredSamples.insert(PianoAction::VK24_L_B2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::B  )));
	
	loweredSamples.insert(PianoAction::VK24_R_C1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::c   )));
	loweredSamples.insert(PianoAction::VK24_R_bD1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::db  )));
	loweredSamples.insert(PianoAction::VK24_R_D1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d   )));
	loweredSamples.insert(PianoAction::VK24_R_bE1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::eb  )));
	loweredSamples.insert(PianoAction::VK24_R_E1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e   )));
	loweredSamples.insert(PianoAction::VK24_R_F1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::f   )));
	loweredSamples.insert(PianoAction::VK24_R_bG1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::gb  )));
	loweredSamples.insert(PianoAction::VK24_R_G1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g   )));
	loweredSamples.insert(PianoAction::VK24_R_bA1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::ab  )));
	loweredSamples.insert(PianoAction::VK24_R_A1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a   )));
	loweredSamples.insert(PianoAction::VK24_R_bB1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::bb  )));
	loweredSamples.insert(PianoAction::VK24_R_B1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b   )));
	
	loweredSamples.insert(PianoAction::VK24_R_C2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::c1  )));
	loweredSamples.insert(PianoAction::VK24_R_bD2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d1b )));
	loweredSamples.insert(PianoAction::VK24_R_D2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d1  )));
	loweredSamples.insert(PianoAction::VK24_R_bE2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e1b )));
	loweredSamples.insert(PianoAction::VK24_R_E2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e1  )));
	loweredSamples.insert(PianoAction::VK24_R_F2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::f1  )));
	loweredSamples.insert(PianoAction::VK24_R_bG2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g1b )));
	loweredSamples.insert(PianoAction::VK24_R_G2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g1  )));
	loweredSamples.insert(PianoAction::VK24_R_bA2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a1b )));
	loweredSamples.insert(PianoAction::VK24_R_A2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a1  )));
	loweredSamples.insert(PianoAction::VK24_R_bB2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b1b )));
	loweredSamples.insert(PianoAction::VK24_R_B2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b1  )));

#pragma endregion

	/* 升八度 */
#pragma region RaisedPitchState

	raisedSamples.insert(PianoAction::VK24_L_C1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::C   )));
	raisedSamples.insert(PianoAction::VK24_L_bD1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Db  )));
	raisedSamples.insert(PianoAction::VK24_L_D1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::D   )));
	raisedSamples.insert(PianoAction::VK24_L_bE1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Eb  )));
	raisedSamples.insert(PianoAction::VK24_L_E1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::E   )));
	raisedSamples.insert(PianoAction::VK24_L_F1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::F   )));
	raisedSamples.insert(PianoAction::VK24_L_bG1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Gb  )));
	raisedSamples.insert(PianoAction::VK24_L_G1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::G   )));
	raisedSamples.insert(PianoAction::VK24_L_bA1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Ab  )));
	raisedSamples.insert(PianoAction::VK24_L_A1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::A   )));
	raisedSamples.insert(PianoAction::VK24_L_bB1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::Bb  )));
	raisedSamples.insert(PianoAction::VK24_L_B1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::B   )));
	
	raisedSamples.insert(PianoAction::VK24_L_C2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::c  )));
	raisedSamples.insert(PianoAction::VK24_L_bD2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::db )));
	raisedSamples.insert(PianoAction::VK24_L_D2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d  )));
	raisedSamples.insert(PianoAction::VK24_L_bE2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::eb )));
	raisedSamples.insert(PianoAction::VK24_L_E2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e  )));
	raisedSamples.insert(PianoAction::VK24_L_F2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::f  )));
	raisedSamples.insert(PianoAction::VK24_L_bG2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::gb )));
	raisedSamples.insert(PianoAction::VK24_L_G2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g  )));
	raisedSamples.insert(PianoAction::VK24_L_bA2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::ab )));
	raisedSamples.insert(PianoAction::VK24_L_A2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a  )));
	raisedSamples.insert(PianoAction::VK24_L_bB2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::bb )));
	raisedSamples.insert(PianoAction::VK24_L_B2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b  )));
	
	raisedSamples.insert(PianoAction::VK24_R_C1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::c1  )));
	raisedSamples.insert(PianoAction::VK24_R_bD1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d1b )));
	raisedSamples.insert(PianoAction::VK24_R_D1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d1  )));
	raisedSamples.insert(PianoAction::VK24_R_bE1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e1b )));
	raisedSamples.insert(PianoAction::VK24_R_E1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e1  )));
	raisedSamples.insert(PianoAction::VK24_R_F1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::f1  )));
	raisedSamples.insert(PianoAction::VK24_R_bG1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g1b )));
	raisedSamples.insert(PianoAction::VK24_R_G1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g1  )));
	raisedSamples.insert(PianoAction::VK24_R_bA1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a1b )));
	raisedSamples.insert(PianoAction::VK24_R_A1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a1  )));
	raisedSamples.insert(PianoAction::VK24_R_bB1 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b1b )));
	raisedSamples.insert(PianoAction::VK24_R_B1  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b1  )));
	
	raisedSamples.insert(PianoAction::VK24_R_C2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::c2  )));
	raisedSamples.insert(PianoAction::VK24_R_bD2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d2b )));
	raisedSamples.insert(PianoAction::VK24_R_D2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::d2  )));
	raisedSamples.insert(PianoAction::VK24_R_bE2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e2b )));
	raisedSamples.insert(PianoAction::VK24_R_E2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::e2  )));
	raisedSamples.insert(PianoAction::VK24_R_F2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::f2  )));
	raisedSamples.insert(PianoAction::VK24_R_bG2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g2b )));
	raisedSamples.insert(PianoAction::VK24_R_G2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::g2  )));
	raisedSamples.insert(PianoAction::VK24_R_bA2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a2b )));
	raisedSamples.insert(PianoAction::VK24_R_A2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::a2  )));
	raisedSamples.insert(PianoAction::VK24_R_bB2 , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b2b )));
	raisedSamples.insert(PianoAction::VK24_R_B2  , audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding(Pitch::b2  )));

#pragma endregion

	return 0;
}
