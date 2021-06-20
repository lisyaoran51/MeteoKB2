#include "MeteoPiano.h"

#include "Input/PianoAction.h"
#include "Pitch.h"
#include "../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "Input/PianoPitchBindingSet.h"
#include "../Games/Output/Panels/IndicatorLightPanelMessage.h"



using namespace Instruments;
using namespace Instruments::Input;
using namespace Games::Output::Bluetooths;
using namespace Games::Output::Panels;




int MeteoPiano::load()
{
	onSleep.Add(this, [=]() {

		LOG(LogLevel::Debug) << "MeteoPiano::Lambda_OnSleep() : " << GetTypeName() << " stop all samples and clean pressing map.";

		ChangePitchState(MeteoPianoPitchState::None);
		SetGameControllingPitchState(false);

		map<PianoAction, SampleChannel*>::iterator it;
		for (it = raisedSamples.begin(); it != raisedSamples.end(); ++it) {

			(*it).second->FadeOut();
		}

		for (it = loweredSamples.begin(); it != loweredSamples.end(); ++it) {

			(*it).second->FadeOut();
		}

		return 0;
	}, "MeteoPiano::Lambda_OnSleep");

	// TODO: 用偵測的，然後需要跟auto sustain和game controlling sustain切換

	sustainType = SustainType::AutoSustain;

	return 0;
}

MeteoPiano::MeteoPiano(vector<string>& args) : RegisterType("MeteoPiano")
{
	registerLoad(bind(static_cast<int(MeteoPiano::*)(void)>(&MeteoPiano::load), this));
}

PitchBindingSet * MeteoPiano::GetDefaultPitchBindingSet(int variant)
{
	return new PianoPitchBindingSet();
}

int MeteoPiano::SetGameControllingPitchState(bool value)
{
	isGameControllingPitchState = value;


	return 0;
}

int MeteoPiano::MoveOctave(PianoPitchMovement m)
{
	IndicatorLightPanelMessage* indicatorLightMessage = nullptr;
	switch (m) {
	case PianoPitchMovement::None:
		break;
	case PianoPitchMovement::Lower:
		switch (state) {
		case MeteoPianoPitchState::Lowered:
			break;
		case MeteoPianoPitchState::None:
			state = MeteoPianoPitchState::Lowered;
			indicatorLightMessage = new IndicatorLightPanelMessage(14, true);
			outputManager->PushMessage(indicatorLightMessage);
			indicatorLightMessage = new IndicatorLightPanelMessage(3, false);
			outputManager->PushMessage(indicatorLightMessage);
			break;
		case MeteoPianoPitchState::Raised:
			state = MeteoPianoPitchState::None;
			indicatorLightMessage = new IndicatorLightPanelMessage(14, false);
			outputManager->PushMessage(indicatorLightMessage);
			indicatorLightMessage = new IndicatorLightPanelMessage(3, false);
			outputManager->PushMessage(indicatorLightMessage);
			break;
		}
		break;
	case PianoPitchMovement::Raise:
		switch (state) {
		case MeteoPianoPitchState::Lowered:
			state = MeteoPianoPitchState::None;
			indicatorLightMessage = new IndicatorLightPanelMessage(14, false);
			outputManager->PushMessage(indicatorLightMessage);
			indicatorLightMessage = new IndicatorLightPanelMessage(3, false);
			outputManager->PushMessage(indicatorLightMessage);
			break;
		case MeteoPianoPitchState::None:
			state = MeteoPianoPitchState::Raised;
			indicatorLightMessage = new IndicatorLightPanelMessage(14, false);
			outputManager->PushMessage(indicatorLightMessage);
			indicatorLightMessage = new IndicatorLightPanelMessage(3, true);
			outputManager->PushMessage(indicatorLightMessage);
			break;
		case MeteoPianoPitchState::Raised:
			break;
		}
		break;
	}

	return 0;
}

int MeteoPiano::ChangePitchState(MeteoPianoPitchState s)
{
	state = s;

	// TODO: 要傳送新的pitch state到手機和mcu

	return 0;
}

int MeteoPiano::OnButtonDown(PianoAction action)
{
	if (isSleeping)
		return -1;

	LOG(LogLevel::Debug) << "MeteoPiano::OnButtonDown() : get button " << (int)action << ".";
	if (!isGameControllingPitchState) {
		MeteoPianoPitchState lastState = state;

		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = nullptr;
		if (action == PianoAction::LowerOctave) {
			MoveOctave(PianoPitchMovement::Lower);
			//meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::PianoPressLowerOctaveButton);
		}
		else if (action == PianoAction::RaiseOctave) {
			MoveOctave(PianoPitchMovement::Raise);
			//meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::PianoPressRaiseOctaveButton);
		}
		
		//outputManager->PushMessage(meteoContextBluetoothMessage);

		if (state != lastState) {
			// 改panel燈號
		}

	}
	return Piano::OnButtonDown(action);
}

int MeteoPiano::OnKnobTurn(pair<PianoAction, int> action)
{
	if (isSleeping)
		return -1;

	return 0;
}

int MeteoPiano::resetState()
{
	// 這邊有點問題，裡面的getSamples會拿到目前音域的sample，但部會拿到所有因欲的Sample，所以這邊只好再把所有因欲都stop調
	Piano::resetState();

	map<PianoAction, SampleChannel*>::iterator it;
	for (it = samples.begin(); it != samples.end(); ++it) {

		(*it).second->Stop();
	}

	for (it = loweredSamples.begin(); it != loweredSamples.end(); ++it) {

		(*it).second->Stop();
	}

	for (it = raisedSamples.begin(); it != raisedSamples.end(); ++it) {

		(*it).second->Stop();
	}

	return 0;
}

map<PianoAction, SampleChannel*>* MeteoPiano::getSamples(int variant)
{
	switch (state) {
	
	case MeteoPianoPitchState::None:
		LOG(LogLevel::Depricated) << "MeteoPiano::getSamples() : get sample of none. ";
		return TInstrument<PianoAction>::getSamples();
		break;
	
	case MeteoPianoPitchState::Lowered:
		LOG(LogLevel::Depricated) << "MeteoPiano::getSamples() : get sample of lowered. ";
		return &loweredSamples;
		break;

	case MeteoPianoPitchState::Raised:
		LOG(LogLevel::Depricated) << "MeteoPiano::getSamples() : get sample of raised. ";
		return &raisedSamples;
		break;
	}
	return nullptr;
}

int MeteoPiano::loadAndMapSamples()
{
	LOG(LogLevel::Info) << "MeteoPiano::loadAndMapSamples() :mapping samplesByPitch.";

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

int MeteoPiano::loadAndMapPitches()
{
	PianoPitchBindingSet* pianoPitchBindingSet = new PianoPitchBindingSet();
	
	/* 未升降 */
#pragma region NonePitchState

	pianoPitchBindingSet->SetPitchBinding(Pitch::C   , PianoAction::VK24_L_C1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::Db  , PianoAction::VK24_L_bD1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::D   , PianoAction::VK24_L_D1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::Eb  , PianoAction::VK24_L_bE1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::E   , PianoAction::VK24_L_E1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::F   , PianoAction::VK24_L_F1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::Gb  , PianoAction::VK24_L_bG1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::G   , PianoAction::VK24_L_G1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::Ab  , PianoAction::VK24_L_bA1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::A   , PianoAction::VK24_L_A1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::Bb  , PianoAction::VK24_L_bB1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::B   , PianoAction::VK24_L_B1 );
		 
	pianoPitchBindingSet->SetPitchBinding(Pitch::c   , PianoAction::VK24_L_C2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::db  , PianoAction::VK24_L_bD2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::d   , PianoAction::VK24_L_D2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::eb  , PianoAction::VK24_L_bE2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::e   , PianoAction::VK24_L_E2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::f   , PianoAction::VK24_L_F2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::gb  , PianoAction::VK24_L_bG2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::g   , PianoAction::VK24_L_G2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::ab  , PianoAction::VK24_L_bA2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::a   , PianoAction::VK24_L_A2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::bb  , PianoAction::VK24_L_bB2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::b   , PianoAction::VK24_L_B2 );
		 
	pianoPitchBindingSet->SetPitchBinding(Pitch::c1  , PianoAction::VK24_R_C1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::d1b , PianoAction::VK24_R_bD1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::d1  , PianoAction::VK24_R_D1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::e1b , PianoAction::VK24_R_bE1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::e1  , PianoAction::VK24_R_E1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::f1  , PianoAction::VK24_R_F1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::g1b , PianoAction::VK24_R_bG1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::g1  , PianoAction::VK24_R_G1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::a1b , PianoAction::VK24_R_bA1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::a1  , PianoAction::VK24_R_A1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::b1b , PianoAction::VK24_R_bB1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::b1  , PianoAction::VK24_R_B1 );
		 
	pianoPitchBindingSet->SetPitchBinding(Pitch::c2  , PianoAction::VK24_R_C2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::d2b , PianoAction::VK24_R_bD2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::d2  , PianoAction::VK24_R_D2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::e2b , PianoAction::VK24_R_bE2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::e2  , PianoAction::VK24_R_E2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::f2  , PianoAction::VK24_R_F2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::g2b , PianoAction::VK24_R_bG2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::g2  , PianoAction::VK24_R_G2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::a2b , PianoAction::VK24_R_bA2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::a2  , PianoAction::VK24_R_A2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::b2b , PianoAction::VK24_R_bB2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::b2  , PianoAction::VK24_R_B2 );


#pragma endregion

	/* 降八度 */
#pragma region LoweredPitchState

	pianoPitchBindingSet->SwitchPitchState(MeteoPianoPitchState::Lowered);

	pianoPitchBindingSet->SetPitchBinding(Pitch::C1  , PianoAction::VK24_L_C1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::D1b , PianoAction::VK24_L_bD1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::D1  , PianoAction::VK24_L_D1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::E1b , PianoAction::VK24_L_bE1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::E1  , PianoAction::VK24_L_E1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::F1  , PianoAction::VK24_L_F1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::G1b , PianoAction::VK24_L_bG1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::G1  , PianoAction::VK24_L_G1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::A1b , PianoAction::VK24_L_bA1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::A1  , PianoAction::VK24_L_A1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::B1b , PianoAction::VK24_L_bB1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::B1  , PianoAction::VK24_L_B1 );

	pianoPitchBindingSet->SetPitchBinding(Pitch::C   , PianoAction::VK24_L_C2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::Db  , PianoAction::VK24_L_bD2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::D   , PianoAction::VK24_L_D2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::Eb  , PianoAction::VK24_L_bE2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::E   , PianoAction::VK24_L_E2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::F   , PianoAction::VK24_L_F2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::Gb  , PianoAction::VK24_L_bG2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::G   , PianoAction::VK24_L_G2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::Ab  , PianoAction::VK24_L_bA2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::A   , PianoAction::VK24_L_A2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::Bb  , PianoAction::VK24_L_bB2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::B   , PianoAction::VK24_L_B2 );

	pianoPitchBindingSet->SetPitchBinding(Pitch::c   , PianoAction::VK24_R_C1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::db  , PianoAction::VK24_R_bD1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::d   , PianoAction::VK24_R_D1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::eb  , PianoAction::VK24_R_bE1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::e   , PianoAction::VK24_R_E1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::f   , PianoAction::VK24_R_F1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::gb  , PianoAction::VK24_R_bG1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::g   , PianoAction::VK24_R_G1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::ab  , PianoAction::VK24_R_bA1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::a   , PianoAction::VK24_R_A1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::bb  , PianoAction::VK24_R_bB1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::b   , PianoAction::VK24_R_B1 );

	pianoPitchBindingSet->SetPitchBinding(Pitch::c1  , PianoAction::VK24_R_C2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::d1b , PianoAction::VK24_R_bD2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::d1  , PianoAction::VK24_R_D2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::e1b , PianoAction::VK24_R_bE2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::e1  , PianoAction::VK24_R_E2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::f1  , PianoAction::VK24_R_F2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::g1b , PianoAction::VK24_R_bG2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::g1  , PianoAction::VK24_R_G2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::a1b , PianoAction::VK24_R_bA2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::a1  , PianoAction::VK24_R_A2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::b1b , PianoAction::VK24_R_bB2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::b1  , PianoAction::VK24_R_B2 );

#pragma endregion

	/* 升八度 */
#pragma region RaisedPitchState

	pianoPitchBindingSet->SwitchPitchState(MeteoPianoPitchState::Raised);

	pianoPitchBindingSet->SetPitchBinding(Pitch::c		, PianoAction::VK24_L_C1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::db		, PianoAction::VK24_L_bD1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::d		, PianoAction::VK24_L_D1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::eb		, PianoAction::VK24_L_bE1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::e		, PianoAction::VK24_L_E1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::f		, PianoAction::VK24_L_F1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::gb		, PianoAction::VK24_L_bG1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::g		, PianoAction::VK24_L_G1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::ab		, PianoAction::VK24_L_bA1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::a		, PianoAction::VK24_L_A1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::bb		, PianoAction::VK24_L_bB1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::b		, PianoAction::VK24_L_B1 );

	pianoPitchBindingSet->SetPitchBinding(Pitch::c1		, PianoAction::VK24_L_C2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::d1b	, PianoAction::VK24_L_bD2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::d1		, PianoAction::VK24_L_D2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::e1b	, PianoAction::VK24_L_bE2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::e1		, PianoAction::VK24_L_E2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::f1		, PianoAction::VK24_L_F2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::g1b	, PianoAction::VK24_L_bG2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::g1		, PianoAction::VK24_L_G2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::a1b	, PianoAction::VK24_L_bA2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::a1		, PianoAction::VK24_L_A2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::b1b	, PianoAction::VK24_L_bB2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::b1		, PianoAction::VK24_L_B2 );

	pianoPitchBindingSet->SetPitchBinding(Pitch::c2	    , PianoAction::VK24_R_C1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::d2b	, PianoAction::VK24_R_bD1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::d2	    , PianoAction::VK24_R_D1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::e2b	, PianoAction::VK24_R_bE1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::e2	    , PianoAction::VK24_R_E1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::f2	    , PianoAction::VK24_R_F1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::g2b	, PianoAction::VK24_R_bG1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::g2	    , PianoAction::VK24_R_G1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::a2b	, PianoAction::VK24_R_bA1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::a2	    , PianoAction::VK24_R_A1 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::b2b	, PianoAction::VK24_R_bB1);
	pianoPitchBindingSet->SetPitchBinding(Pitch::b2	    , PianoAction::VK24_R_B1 ); 

	pianoPitchBindingSet->SetPitchBinding(Pitch::c3	    , PianoAction::VK24_R_C2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::d3b	, PianoAction::VK24_R_bD2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::d3	    , PianoAction::VK24_R_D2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::e3b	, PianoAction::VK24_R_bE2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::e3	    , PianoAction::VK24_R_E2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::f3	    , PianoAction::VK24_R_F2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::g3b	, PianoAction::VK24_R_bG2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::g3	    , PianoAction::VK24_R_G2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::a3b	, PianoAction::VK24_R_bA2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::a3	    , PianoAction::VK24_R_A2 );
	pianoPitchBindingSet->SetPitchBinding(Pitch::b3b	, PianoAction::VK24_R_bB2);
	pianoPitchBindingSet->SetPitchBinding(Pitch::b3	    , PianoAction::VK24_R_B2 );

#pragma endregion

	pianoPitchBindingSet->SwitchPitchState(MeteoPianoPitchState::None);

	pitchBindingSet = pianoPitchBindingSet;

	return 0;
}
