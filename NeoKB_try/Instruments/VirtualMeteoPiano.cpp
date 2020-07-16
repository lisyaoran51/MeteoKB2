#include "VirtualMeteoPiano.h"

#include "../Framework/Audio/Sample/MultiPlaybackSampleChannel.h"

using namespace Instruments;
using namespace Framework::Audio::Samples;


VirtualMeteoPiano::VirtualMeteoPiano() : RegisterType("VirtualMeteoPiano")
{
}

int VirtualMeteoPiano::SetSustainType(VirtualMeteoPianoSustainType sType)
{
	return 0;
}

int VirtualMeteoPiano::Play(pair<PianoAction, int> action)
{
	if (!dynamic_cast<MultiPlaybackSampleChannel*>(getSamples()->at(action.first)))
		return 0;

	dynamic_cast<MultiPlaybackSampleChannel*>(getSamples()->at(action.first))->PlayOnTrack(1, double(action.second) / 256.0);

	isPressingMap[action.first] = true;

	return 0;
}

int VirtualMeteoPiano::Stop(PianoAction action)
{
	if (sustainType != VirtualMeteoPianoSustainType::Pedal)
		return 0;

	if (!dynamic_cast<MultiPlaybackSampleChannel*>(getSamples()->at(action)))
		return 0;

	if (!isPressingMap.at(PianoAction::SustainPedal))
		dynamic_cast<MultiPlaybackSampleChannel*>(getSamples()->at(action))->FadeOut(1);

	


	isPressingMap[action] = false;

	return 0;
}

int VirtualMeteoPiano::PressPedal()
{
	if (sustainType != VirtualMeteoPianoSustainType::Pedal)
		return 0;

	isPressingMap[PianoAction::SustainPedal] = true;


	return 0;
}

int VirtualMeteoPiano::ReleasePedal()
{
	if (sustainType != VirtualMeteoPianoSustainType::Pedal)
		return 0;

	isPressingMap[PianoAction::SustainPedal] = false;

	map<PianoAction, bool>::iterator it;
	for (it = isPressingMap.begin(); it != isPressingMap.end(); ++it) {
		if (!it->second) {

			if (getSamples()->find(it->first) != getSamples()->end()) {

				MultiPlaybackSampleChannel* sampleChannel = dynamic_cast<MultiPlaybackSampleChannel*>(getSamples()->at(it->first));
				if (sampleChannel) {
					if (sampleChannel->GetIsPlaying(1))
						sampleChannel->FadeOut(1);
				}
			}
		}
	}


	return 0;
}

int VirtualMeteoPiano::MoveOctave(PianoPitchMovement m)
{
	// 在這邊沒用
	return 0;
}

map<PianoAction, SampleChannel*>* VirtualMeteoPiano::getSamples(int variant)
{
	return TInstrument<PianoAction>::getSamples();
}

int VirtualMeteoPiano::loadAndMapSamples()
{

	// 這一段跟MeteoPiano依樣，有點不好
	// 寫錯了，要用pitch來當key，晚點改
	samples[PianoAction::VK24_L_C1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::C  ));
	samples[PianoAction::VK24_L_bD1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::Db ));
	samples[PianoAction::VK24_L_D1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::D  ));
	samples[PianoAction::VK24_L_bE1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::Eb ));
	samples[PianoAction::VK24_L_E1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::E  ));
	samples[PianoAction::VK24_L_F1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::F  ));
	samples[PianoAction::VK24_L_bG1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::Gb ));
	samples[PianoAction::VK24_L_G1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::G  ));
	samples[PianoAction::VK24_L_bA1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::Ab ));
	samples[PianoAction::VK24_L_A1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::A  ));
	samples[PianoAction::VK24_L_bB1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::Bb ));
	samples[PianoAction::VK24_L_B1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::B  ));

	samples[PianoAction::VK24_L_C2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::c  ));
	samples[PianoAction::VK24_L_bD2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::db ));
	samples[PianoAction::VK24_L_D2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d  ));
	samples[PianoAction::VK24_L_bE2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::eb ));
	samples[PianoAction::VK24_L_E2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e  ));
	samples[PianoAction::VK24_L_F2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::f  ));
	samples[PianoAction::VK24_L_bG2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::gb ));
	samples[PianoAction::VK24_L_G2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g  ));
	samples[PianoAction::VK24_L_bA2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::ab ));
	samples[PianoAction::VK24_L_A2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a  ));
	samples[PianoAction::VK24_L_bB2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::bb ));
	samples[PianoAction::VK24_L_B2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b  ));

	samples[PianoAction::VK24_R_C1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::c1 ));
	samples[PianoAction::VK24_R_bD1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d1b));
	samples[PianoAction::VK24_R_D1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d1 ));
	samples[PianoAction::VK24_R_bE1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e1b));
	samples[PianoAction::VK24_R_E1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e1 ));
	samples[PianoAction::VK24_R_F1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::f1 ));
	samples[PianoAction::VK24_R_bG1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g1b));
	samples[PianoAction::VK24_R_G1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g1 ));
	samples[PianoAction::VK24_R_bA1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a1b));
	samples[PianoAction::VK24_R_A1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a1 ));
	samples[PianoAction::VK24_R_bB1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b1b));
	samples[PianoAction::VK24_R_B1  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b1 ));

	samples[PianoAction::VK24_R_C2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::c2 ));
	samples[PianoAction::VK24_R_bD2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d2b));
	samples[PianoAction::VK24_R_D2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d2 ));
	samples[PianoAction::VK24_R_bE2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e2b));
	samples[PianoAction::VK24_R_E2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e2 ));
	samples[PianoAction::VK24_R_F2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::f2 ));
	samples[PianoAction::VK24_R_bG2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g2b));
	samples[PianoAction::VK24_R_G2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g2 ));
	samples[PianoAction::VK24_R_bA2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a2b));
	samples[PianoAction::VK24_R_A2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a2 ));
	samples[PianoAction::VK24_R_bB2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b2b));
	samples[PianoAction::VK24_R_B2  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b2 ));

	return 0;
}

int VirtualMeteoPiano::OnKeyDown(pair<PianoAction, int> action)
{
	return 0;
}

int VirtualMeteoPiano::OnKeyUp(PianoAction action)
{
	return 0;
}

int VirtualMeteoPiano::OnButtonDown(PianoAction action)
{
	return 0;
}

int VirtualMeteoPiano::OnButtonUp(PianoAction action)
{
	return 0;
}

int VirtualMeteoPiano::OnKnobTurn(pair<PianoAction, int> action)
{
	return 0;
}

int VirtualMeteoPiano::OnSlide(pair<PianoAction, int> action)
{
	return 0;
}
