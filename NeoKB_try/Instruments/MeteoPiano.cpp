#include "MeteoPiano.h"

#include "Input/PianoAction.h"
#include "Pitch.h"


using namespace Instruments;
using namespace Instruments::Input;




MeteoPiano::MeteoPiano(vector<string>& args) : RegisterType("MeteoPiano")
{
}

int MeteoPiano::SetGameControllingPitchState(bool value)
{
	isGameControllingPitchState = value;
	return 0;
}

int MeteoPiano::ChangePitchState(MeteoPianoPitchState s)
{
	state = s;
	return 0;
}

int MeteoPiano::OnButtonDown(PianoAction action)
{
	LOG(LogLevel::Debug) << "MeteoPiano::OnButtonDown() : get button " << (int)action << ".";
	if (!isGameControllingPitchState) {
		if (action == PianoAction::LowerOctave) {
			switch (state) {
			case MeteoPianoPitchState::Lowered:
				break;
			case MeteoPianoPitchState::None:
				state = MeteoPianoPitchState::Lowered;
				break;
			case MeteoPianoPitchState::Raised:
				state = MeteoPianoPitchState::None;
				break;
			}
		}
		else if (action == PianoAction::RaiseOctave) {
			switch (state) {
			case MeteoPianoPitchState::Lowered:
				state = MeteoPianoPitchState::None;
				break;
			case MeteoPianoPitchState::None:
				state = MeteoPianoPitchState::Raised;
				break;
			case MeteoPianoPitchState::Raised:
				break;
			}
		}
	}
	return Piano::OnButtonDown(action);
}

int MeteoPiano::OnKnobTurn(pair<PianoAction, int> action)
{
	return 0;
}

map<PianoAction, SampleChannel*>* MeteoPiano::getSamples()
{
	switch (state) {
	
	case MeteoPianoPitchState::None:
		LOG(LogLevel::Debug) << "MeteoPiano::getSamples() : get sample of none. ";
		return TInstrument<PianoAction>::getSamples();
		break;
	
	case MeteoPianoPitchState::Lowered:
		LOG(LogLevel::Debug) << "MeteoPiano::getSamples() : get sample of lowered. ";
		return &loweredSamples;
		break;

	case MeteoPianoPitchState::Raised:
		LOG(LogLevel::Debug) << "MeteoPiano::getSamples() : get sample of raised. ";
		return &raisedSamples;
		break;
	}
	return nullptr;
}

int MeteoPiano::loadAndMapSamples()
{

	/* 未升降 */
#pragma region NonePitchState


	samples[PianoAction::VK24_L_C1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::C  ));
	samples[PianoAction::VK24_L_bD1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Db ));
	samples[PianoAction::VK24_L_D1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::D  ));
	samples[PianoAction::VK24_L_bE1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Eb ));
	samples[PianoAction::VK24_L_E1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::E  ));
	samples[PianoAction::VK24_L_F1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::F  ));
	samples[PianoAction::VK24_L_bG1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Gb ));
	samples[PianoAction::VK24_L_G1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::G  ));
	samples[PianoAction::VK24_L_bA1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Ab ));
	samples[PianoAction::VK24_L_A1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::A  ));
	samples[PianoAction::VK24_L_bB1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Bb ));
	samples[PianoAction::VK24_L_B1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::B  ));

	samples[PianoAction::VK24_L_C2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c  ));
	samples[PianoAction::VK24_L_bD2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::db ));
	samples[PianoAction::VK24_L_D2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d  ));
	samples[PianoAction::VK24_L_bE2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::eb ));
	samples[PianoAction::VK24_L_E2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e  ));
	samples[PianoAction::VK24_L_F2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f  ));
	samples[PianoAction::VK24_L_bG2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::gb ));
	samples[PianoAction::VK24_L_G2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g  ));
	samples[PianoAction::VK24_L_bA2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::ab ));
	samples[PianoAction::VK24_L_A2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a  ));
	samples[PianoAction::VK24_L_bB2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::bb ));
	samples[PianoAction::VK24_L_B2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b  ));

	samples[PianoAction::VK24_R_C1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c1 ));
	samples[PianoAction::VK24_R_bD1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d1b));
	samples[PianoAction::VK24_R_D1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d1 ));
	samples[PianoAction::VK24_R_bE1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e1b));
	samples[PianoAction::VK24_R_E1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e1 ));
	samples[PianoAction::VK24_R_F1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f1 ));
	samples[PianoAction::VK24_R_bG1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g1b));
	samples[PianoAction::VK24_R_G1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g1 ));
	samples[PianoAction::VK24_R_bA1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a1b));
	samples[PianoAction::VK24_R_A1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a1 ));
	samples[PianoAction::VK24_R_bB1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b1b));
	samples[PianoAction::VK24_R_B1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b1 ));

	samples[PianoAction::VK24_R_C2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c2 ));
	samples[PianoAction::VK24_R_bD2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d2b));
	samples[PianoAction::VK24_R_D2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d2 ));
	samples[PianoAction::VK24_R_bE2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e2b));
	samples[PianoAction::VK24_R_E2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e2 ));
	samples[PianoAction::VK24_R_F2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f2 ));
	samples[PianoAction::VK24_R_bG2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g2b));
	samples[PianoAction::VK24_R_G2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g2 ));
	samples[PianoAction::VK24_R_bA2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a2b));
	samples[PianoAction::VK24_R_A2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a2 ));
	samples[PianoAction::VK24_R_bB2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b2b));
	samples[PianoAction::VK24_R_B2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b2 ));


#pragma endregion

	/* 降八度 */
#pragma region LoweredPitchState

	loweredSamples[PianoAction::VK24_L_C1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::C1 ));
	loweredSamples[PianoAction::VK24_L_bD1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::D1b));
	loweredSamples[PianoAction::VK24_L_D1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::D1 ));
	loweredSamples[PianoAction::VK24_L_bE1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::E1b));
	loweredSamples[PianoAction::VK24_L_E1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::E1 ));
	loweredSamples[PianoAction::VK24_L_F1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::F1 ));
	loweredSamples[PianoAction::VK24_L_bG1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::G1b));
	loweredSamples[PianoAction::VK24_L_G1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::G1 ));
	loweredSamples[PianoAction::VK24_L_bA1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::A1b));
	loweredSamples[PianoAction::VK24_L_A1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::A1 ));
	loweredSamples[PianoAction::VK24_L_bB1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::B1b));
	loweredSamples[PianoAction::VK24_L_B1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::B1 ));
		   
	loweredSamples[PianoAction::VK24_L_C2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::C  ));
	loweredSamples[PianoAction::VK24_L_bD2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Db ));
	loweredSamples[PianoAction::VK24_L_D2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::D  ));
	loweredSamples[PianoAction::VK24_L_bE2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Eb ));
	loweredSamples[PianoAction::VK24_L_E2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::E  ));
	loweredSamples[PianoAction::VK24_L_F2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::F  ));
	loweredSamples[PianoAction::VK24_L_bG2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Gb ));
	loweredSamples[PianoAction::VK24_L_G2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::G  ));
	loweredSamples[PianoAction::VK24_L_bA2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Ab ));
	loweredSamples[PianoAction::VK24_L_A2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::A  ));
	loweredSamples[PianoAction::VK24_L_bB2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Bb ));
	loweredSamples[PianoAction::VK24_L_B2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::B  ));
		   
	loweredSamples[PianoAction::VK24_R_C1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c  ));
	loweredSamples[PianoAction::VK24_R_bD1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::db ));
	loweredSamples[PianoAction::VK24_R_D1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d  ));
	loweredSamples[PianoAction::VK24_R_bE1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::eb ));
	loweredSamples[PianoAction::VK24_R_E1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e  ));
	loweredSamples[PianoAction::VK24_R_F1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f  ));
	loweredSamples[PianoAction::VK24_R_bG1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::gb ));
	loweredSamples[PianoAction::VK24_R_G1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g  ));
	loweredSamples[PianoAction::VK24_R_bA1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::ab ));
	loweredSamples[PianoAction::VK24_R_A1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a  ));
	loweredSamples[PianoAction::VK24_R_bB1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::bb ));
	loweredSamples[PianoAction::VK24_R_B1  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b  ));
		   
	loweredSamples[PianoAction::VK24_R_C2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c1 ));
	loweredSamples[PianoAction::VK24_R_bD2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d1b));
	loweredSamples[PianoAction::VK24_R_D2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d1 ));
	loweredSamples[PianoAction::VK24_R_bE2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e1b));
	loweredSamples[PianoAction::VK24_R_E2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e1 ));
	loweredSamples[PianoAction::VK24_R_F2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f1 ));
	loweredSamples[PianoAction::VK24_R_bG2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g1b));
	loweredSamples[PianoAction::VK24_R_G2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g1 ));
	loweredSamples[PianoAction::VK24_R_bA2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a1b));
	loweredSamples[PianoAction::VK24_R_A2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a1 ));
	loweredSamples[PianoAction::VK24_R_bB2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b1b));
	loweredSamples[PianoAction::VK24_R_B2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b1 ));

#pragma endregion

	/* 升八度 */
#pragma region RaisedPitchState

	raisedSamples[PianoAction::VK24_L_C1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c ));
	raisedSamples[PianoAction::VK24_L_bD1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::db));
	raisedSamples[PianoAction::VK24_L_D1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d ));
	raisedSamples[PianoAction::VK24_L_bE1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::eb));
	raisedSamples[PianoAction::VK24_L_E1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e ));
	raisedSamples[PianoAction::VK24_L_F1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f ));
	raisedSamples[PianoAction::VK24_L_bG1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::gb));
	raisedSamples[PianoAction::VK24_L_G1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g ));
	raisedSamples[PianoAction::VK24_L_bA1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::ab));
	raisedSamples[PianoAction::VK24_L_A1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a ));
	raisedSamples[PianoAction::VK24_L_bB1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::bb));
	raisedSamples[PianoAction::VK24_L_B1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b ));
		  
	raisedSamples[PianoAction::VK24_L_C2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c1 ));
	raisedSamples[PianoAction::VK24_L_bD2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d1b));
	raisedSamples[PianoAction::VK24_L_D2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d1 ));
	raisedSamples[PianoAction::VK24_L_bE2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e1b));
	raisedSamples[PianoAction::VK24_L_E2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e1 ));
	raisedSamples[PianoAction::VK24_L_F2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f1 ));
	raisedSamples[PianoAction::VK24_L_bG2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g1b));
	raisedSamples[PianoAction::VK24_L_G2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g1 ));
	raisedSamples[PianoAction::VK24_L_bA2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a1b));
	raisedSamples[PianoAction::VK24_L_A2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a1 ));
	raisedSamples[PianoAction::VK24_L_bB2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b1b));
	raisedSamples[PianoAction::VK24_L_B2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b1 ));
		  
	raisedSamples[PianoAction::VK24_R_C1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c2 ));
	raisedSamples[PianoAction::VK24_R_bD1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d2b));
	raisedSamples[PianoAction::VK24_R_D1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d2 ));
	raisedSamples[PianoAction::VK24_R_bE1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e2b));
	raisedSamples[PianoAction::VK24_R_E1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e2 ));
	raisedSamples[PianoAction::VK24_R_F1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f2 ));
	raisedSamples[PianoAction::VK24_R_bG1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g2b));
	raisedSamples[PianoAction::VK24_R_G1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g2 ));
	raisedSamples[PianoAction::VK24_R_bA1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a2b));
	raisedSamples[PianoAction::VK24_R_A1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a2 ));
	raisedSamples[PianoAction::VK24_R_bB1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b2b));
	raisedSamples[PianoAction::VK24_R_B1]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b2 ));
		  
	raisedSamples[PianoAction::VK24_R_C2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c3 ));
	raisedSamples[PianoAction::VK24_R_bD2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d3b));
	raisedSamples[PianoAction::VK24_R_D2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d3 ));
	raisedSamples[PianoAction::VK24_R_bE2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e3b));
	raisedSamples[PianoAction::VK24_R_E2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e3 ));
	raisedSamples[PianoAction::VK24_R_F2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f3 ));
	raisedSamples[PianoAction::VK24_R_bG2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g3b));
	raisedSamples[PianoAction::VK24_R_G2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g3 ));
	raisedSamples[PianoAction::VK24_R_bA2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a3b));
	raisedSamples[PianoAction::VK24_R_A2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a3 ));
	raisedSamples[PianoAction::VK24_R_bB2]	= audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b3b));
	raisedSamples[PianoAction::VK24_R_B2  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b3 ));

#pragma endregion

	return 0;
}
