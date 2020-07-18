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


	//LOG(LogLevel::Fine) << "VirtualMeteoPiano::Play() : play sound [" << dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p)) << "].";
	//if (!dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p)))
	//	return 0;
	//
	//dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p))->PlayOnTrack(1, volume);

	getSamplesByPitch()->at(p)->Play(volume);

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

				//MultiPlaybackSampleChannel* sampleChannel = dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(it->first));
				//if (sampleChannel) {
				//	if (sampleChannel->GetIsPlaying(1))
				//		sampleChannel->FadeOut(1);
				//}
				if(getSamplesByPitch()->at(it->first)->GetIsPlaying())
					getSamplesByPitch()->at(it->first)->FadeOut();

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
	/*
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

	samplesByPitch[Pitch::c  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::c  ));
	samplesByPitch[Pitch::db ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::db ));
	samplesByPitch[Pitch::d  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d  ));
	samplesByPitch[Pitch::eb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::eb ));
	samplesByPitch[Pitch::e  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e  ));
	samplesByPitch[Pitch::f  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::f  ));
	samplesByPitch[Pitch::gb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::gb ));
	samplesByPitch[Pitch::g  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g  ));
	samplesByPitch[Pitch::ab ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::ab ));
	samplesByPitch[Pitch::a  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a  ));
	samplesByPitch[Pitch::bb ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::bb ));
	samplesByPitch[Pitch::b  ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b  ));
		   
	samplesByPitch[Pitch::c1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::c1 ));
	samplesByPitch[Pitch::d1b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d1b));
	samplesByPitch[Pitch::d1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d1 ));
	samplesByPitch[Pitch::e1b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e1b));
	samplesByPitch[Pitch::e1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e1 ));
	samplesByPitch[Pitch::f1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::f1 ));
	samplesByPitch[Pitch::g1b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g1b));
	samplesByPitch[Pitch::g1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g1 ));
	samplesByPitch[Pitch::a1b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a1b));
	samplesByPitch[Pitch::a1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a1 ));
	samplesByPitch[Pitch::b1b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b1b));
	samplesByPitch[Pitch::b1 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b1 ));
		   
	samplesByPitch[Pitch::c2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::c2 ));
	samplesByPitch[Pitch::d2b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d2b));
	samplesByPitch[Pitch::d2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::d2 ));
	samplesByPitch[Pitch::e2b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e2b));
	samplesByPitch[Pitch::e2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::e2 ));
	samplesByPitch[Pitch::f2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::f2 ));
	samplesByPitch[Pitch::g2b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g2b));
	samplesByPitch[Pitch::g2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::g2 ));
	samplesByPitch[Pitch::a2b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a2b));
	samplesByPitch[Pitch::a2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::a2 ));
	samplesByPitch[Pitch::b2b] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b2b));
	samplesByPitch[Pitch::b2 ] = audioManager->GetSampleManager()->GetMultiPlaybackSampleChannel(getSoundBinding((int)Pitch::b2 ));

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
	*/
	
	samplesByPitch[Pitch::C1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::C1 ));
	samplesByPitch[Pitch::D1b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::D1b));
	samplesByPitch[Pitch::D1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::D1 ));
	samplesByPitch[Pitch::E1b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::E1b));
	samplesByPitch[Pitch::E1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::E1 ));
	samplesByPitch[Pitch::F1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::F1 ));
	samplesByPitch[Pitch::G1b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::G1b));
	samplesByPitch[Pitch::G1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::G1 ));
	samplesByPitch[Pitch::A1b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::A1b));
	samplesByPitch[Pitch::A1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::A1 ));
	samplesByPitch[Pitch::B1b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::B1b));
	samplesByPitch[Pitch::B1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::B1 ));
																   
	samplesByPitch[Pitch::C  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::C  ));
	samplesByPitch[Pitch::Db ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Db ));
	samplesByPitch[Pitch::D  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::D  ));
	samplesByPitch[Pitch::Eb ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Eb ));
	samplesByPitch[Pitch::E  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::E  ));
	samplesByPitch[Pitch::F  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::F  ));
	samplesByPitch[Pitch::Gb ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Gb ));
	samplesByPitch[Pitch::G  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::G  ));
	samplesByPitch[Pitch::Ab ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Ab ));
	samplesByPitch[Pitch::A  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::A  ));
	samplesByPitch[Pitch::Bb ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Bb ));
	samplesByPitch[Pitch::B  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::B  ));
																   
	samplesByPitch[Pitch::c  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c  ));
	samplesByPitch[Pitch::db ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::db ));
	samplesByPitch[Pitch::d  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d  ));
	samplesByPitch[Pitch::eb ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::eb ));
	samplesByPitch[Pitch::e  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e  ));
	samplesByPitch[Pitch::f  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f  ));
	samplesByPitch[Pitch::gb ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::gb ));
	samplesByPitch[Pitch::g  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g  ));
	samplesByPitch[Pitch::ab ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::ab ));
	samplesByPitch[Pitch::a  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a  ));
	samplesByPitch[Pitch::bb ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::bb ));
	samplesByPitch[Pitch::b  ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b  ));
		   														   
	samplesByPitch[Pitch::c1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c1 ));
	samplesByPitch[Pitch::d1b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d1b));
	samplesByPitch[Pitch::d1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d1 ));
	samplesByPitch[Pitch::e1b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e1b));
	samplesByPitch[Pitch::e1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e1 ));
	samplesByPitch[Pitch::f1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f1 ));
	samplesByPitch[Pitch::g1b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g1b));
	samplesByPitch[Pitch::g1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g1 ));
	samplesByPitch[Pitch::a1b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a1b));
	samplesByPitch[Pitch::a1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a1 ));
	samplesByPitch[Pitch::b1b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b1b));
	samplesByPitch[Pitch::b1 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b1 ));
		   														   
	samplesByPitch[Pitch::c2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c2 ));
	samplesByPitch[Pitch::d2b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d2b));
	samplesByPitch[Pitch::d2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d2 ));
	samplesByPitch[Pitch::e2b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e2b));
	samplesByPitch[Pitch::e2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e2 ));
	samplesByPitch[Pitch::f2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f2 ));
	samplesByPitch[Pitch::g2b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g2b));
	samplesByPitch[Pitch::g2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g2 ));
	samplesByPitch[Pitch::a2b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a2b));
	samplesByPitch[Pitch::a2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a2 ));
	samplesByPitch[Pitch::b2b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b2b));
	samplesByPitch[Pitch::b2 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b2 ));
																   
	samplesByPitch[Pitch::c3 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c3 ));
	samplesByPitch[Pitch::d3b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d3b));
	samplesByPitch[Pitch::d3 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d3 ));
	samplesByPitch[Pitch::e3b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e3b));
	samplesByPitch[Pitch::e3 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e3 ));
	samplesByPitch[Pitch::f3 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f3 ));
	samplesByPitch[Pitch::g3b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g3b));
	samplesByPitch[Pitch::g3 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g3 ));
	samplesByPitch[Pitch::a3b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a3b));
	samplesByPitch[Pitch::a3 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a3 ));
	samplesByPitch[Pitch::b3b] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b3b));
	samplesByPitch[Pitch::b3 ] = audioManager->GetSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b3 ));


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
