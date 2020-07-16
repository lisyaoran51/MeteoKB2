#include "VirtualMeteoPiano.h"

#include "../Framework/Audio/Sample/MultiPlaybackSampleChannel.h"

using namespace Instruments;
using namespace Framework::Audio::Samples;


VirtualMeteoPiano::VirtualMeteoPiano() : RegisterType("VirtualMeteoPiano")
{
}

int VirtualMeteoPiano::SetSustainType(VirtualMeteoPianoSustainType sType)
{
	sustainType = sType;

	return 0;
}

int VirtualMeteoPiano::Play(Pitch p, float volume)
{
	if (getSamplesByPitch()->find(p) == getSamplesByPitch()->end()) {
		LOG(LogLevel::Error) << "VirtualMeteoPiano::Play() : sound [" << int(p) << "] not found in samplesByPitch.";
		return 0;
	}


	LOG(LogLevel::Debug) << "VirtualMeteoPiano::Play() : play sound [" << dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p)) << "].";
	if (!dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p)))
		return 0;

	dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p))->PlayOnTrack(1, volume);

	isPressingMapByPitch[p] = true;

	return 0;
}

int VirtualMeteoPiano::Stop(Pitch p)
{
	if (sustainType != VirtualMeteoPianoSustainType::Pedal)
		return 0;

	if (!dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p)))
		return 0;

	if (!pedalDown)
		dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p))->FadeOut(1);


	isPressingMapByPitch[p] = false;

	return 0;
}

int VirtualMeteoPiano::PressPedal()
{
	if (sustainType != VirtualMeteoPianoSustainType::Pedal)
		return 0;

	pedalDown = true;


	return 0;
}

int VirtualMeteoPiano::ReleasePedal()
{
	if (sustainType != VirtualMeteoPianoSustainType::Pedal)
		return 0;

	pedalDown = false;

	map<Pitch, bool>::iterator it;
	for (it = isPressingMapByPitch.begin(); it != isPressingMapByPitch.end(); ++it) {
		if (!it->second) {

			if (getSamplesByPitch()->find(it->first) != getSamplesByPitch()->end()) {

				MultiPlaybackSampleChannel* sampleChannel = dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(it->first));
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
	throw runtime_error("VirtualMeteoPiano::MoveOctave() : this function is not available in VirtualMeteoPiano.");
	return 0;
}

map<PianoAction, SampleChannel*>* VirtualMeteoPiano::getSamples(int variant)
{
	// 應該要土錯誤
	throw runtime_error("VirtualMeteoPiano::getSamples() : this function is not available in VirtualMeteoPiano.");
	return TInstrument<PianoAction>::getSamples();
}

map<Pitch, SampleChannel*>* VirtualMeteoPiano::getSamplesByPitch(int variant)
{
	return &samplesByPitch;
}

int VirtualMeteoPiano::loadAndMapSamples()
{
	LOG(LogLevel::Info) << "VirtualMeteoPiano::loadAndMapSamples() :mapping samplesByPitch.";

	samplesByPitch[Pitch::C1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::C1 ));
	samplesByPitch[Pitch::D1b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::D1b));
	samplesByPitch[Pitch::D1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::D1 ));
	samplesByPitch[Pitch::E1b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::E1b));
	samplesByPitch[Pitch::E1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::E1 ));
	samplesByPitch[Pitch::F1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::F1 ));
	samplesByPitch[Pitch::G1b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::G1b));
	samplesByPitch[Pitch::G1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::G1 ));
	samplesByPitch[Pitch::A1b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::A1b));
	samplesByPitch[Pitch::A1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::A1 ));
	samplesByPitch[Pitch::B1b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::B1b));
	samplesByPitch[Pitch::B1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::B1 ));

	samplesByPitch[Pitch::C  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::C  ));
	samplesByPitch[Pitch::Db ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::Db ));
	samplesByPitch[Pitch::D  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::D  ));
	samplesByPitch[Pitch::Eb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::Eb ));
	samplesByPitch[Pitch::E  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::E  ));
	samplesByPitch[Pitch::F  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::F  ));
	samplesByPitch[Pitch::Gb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::Gb ));
	samplesByPitch[Pitch::G  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::G  ));
	samplesByPitch[Pitch::Ab ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::Ab ));
	samplesByPitch[Pitch::A  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::A  ));
	samplesByPitch[Pitch::Bb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::Bb ));
	samplesByPitch[Pitch::B  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::B  ));

	samplesByPitch[Pitch::C  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::c  ));
	samplesByPitch[Pitch::Db ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::db ));
	samplesByPitch[Pitch::D  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d  ));
	samplesByPitch[Pitch::Eb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::eb ));
	samplesByPitch[Pitch::E  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e  ));
	samplesByPitch[Pitch::F  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::f  ));
	samplesByPitch[Pitch::Gb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::gb ));
	samplesByPitch[Pitch::G  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g  ));
	samplesByPitch[Pitch::Ab ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::ab ));
	samplesByPitch[Pitch::A  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a  ));
	samplesByPitch[Pitch::Bb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::bb ));
	samplesByPitch[Pitch::B  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b  ));
		   
	samplesByPitch[Pitch::C  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::c1 ));
	samplesByPitch[Pitch::Db ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d1b));
	samplesByPitch[Pitch::D  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d1 ));
	samplesByPitch[Pitch::Eb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e1b));
	samplesByPitch[Pitch::E  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e1 ));
	samplesByPitch[Pitch::F  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::f1 ));
	samplesByPitch[Pitch::Gb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g1b));
	samplesByPitch[Pitch::G  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g1 ));
	samplesByPitch[Pitch::Ab ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a1b));
	samplesByPitch[Pitch::A  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a1 ));
	samplesByPitch[Pitch::Bb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b1b));
	samplesByPitch[Pitch::B  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b1 ));
		   
	samplesByPitch[Pitch::C  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::c2 ));
	samplesByPitch[Pitch::Db ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d2b));
	samplesByPitch[Pitch::D  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d2 ));
	samplesByPitch[Pitch::Eb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e2b));
	samplesByPitch[Pitch::E  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e2 ));
	samplesByPitch[Pitch::F  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::f2 ));
	samplesByPitch[Pitch::Gb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g2b));
	samplesByPitch[Pitch::G  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g2 ));
	samplesByPitch[Pitch::Ab ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a2b));
	samplesByPitch[Pitch::A  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a2 ));
	samplesByPitch[Pitch::Bb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b2b));
	samplesByPitch[Pitch::B  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b2 ));

	samplesByPitch[Pitch::c3 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::c3 ));
	samplesByPitch[Pitch::d3b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d3b));
	samplesByPitch[Pitch::d3 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d3 ));
	samplesByPitch[Pitch::e3b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e3b));
	samplesByPitch[Pitch::e3 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e3 ));
	samplesByPitch[Pitch::f3 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::f3 ));
	samplesByPitch[Pitch::g3b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g3b));
	samplesByPitch[Pitch::g3 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g3 ));
	samplesByPitch[Pitch::a3b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a3b));
	samplesByPitch[Pitch::a3 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a3 ));
	samplesByPitch[Pitch::b3b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b3b));
	samplesByPitch[Pitch::b3 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b3 ));

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
