#include "VirtualMeteoPiano.h"

#include "../Framework/Audio/Sample/MultiPlaybackSampleChannel.h"

// debug用
#include <chrono>
using namespace std::chrono;
// debug用

using namespace Instruments;
using namespace Framework::Audio::Samples;


VirtualMeteoPiano::VirtualMeteoPiano() : RegisterType("VirtualMeteoPiano")
{
}

PitchBindingSet * VirtualMeteoPiano::GetDefaultPitchBindingSet(int variant)
{
	return nullptr;
}

int VirtualMeteoPiano::Sleep()
{
	if (isSleeping)
		return -1;
	Piano::Sleep();

	SetVirtualMeteoPianoSustainType(VirtualMeteoPianoSustainType::Auto);

	return 0;
}

int VirtualMeteoPiano::WakeUp()
{
	if (!isSleeping)
		return -1;
	Piano::WakeUp();

	return 0;
}

int VirtualMeteoPiano::SetVirtualMeteoPianoSustainType(VirtualMeteoPianoSustainType sType)
{
	sustainType = sType;

	return 0;
}

VirtualMeteoPianoSustainType VirtualMeteoPiano::GetVirtualMeteoPianoSustainType()
{
	return sustainType;
}

int VirtualMeteoPiano::Play(Pitch p, float volume)
{
	if (isSleeping)
		return -1;

	/* 先檢查是否可以輸入，可以的時候才能控制 */
	if (!isActive)
		return 0;

	typename map<Pitch, SampleChannel*>::iterator it = getSamplesByPitch()->find(p);
	if (it == getSamplesByPitch()->end()) {
		LOG(LogLevel::Error) << "VirtualMeteoPiano::Play() : sound [" << int(p) << "] not found in samplesByPitch.";
		return 0;
	}


	//LOG(LogLevel::Fine) << "VirtualMeteoPiano::Play() : play sound [" << dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p)) << "].";
	//if (!dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p)))
	//	return 0;
	//
	//dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p))->PlayOnTrack(1, volume);


	system_clock::time_point systemStartTime = system_clock::now();
	(*it).second->Play(volume);
	system_clock::time_point systemCurrentTime = system_clock::now();
	LOG(LogLevel::Fine) << "VirtualMeteoPiano::Play() : play sound cost time = [" << duration_cast<microseconds>(systemCurrentTime - systemStartTime).count() << "].";

	isPressingMapByPitch[p] = true;

	return 0;
}

int VirtualMeteoPiano::Stop(Pitch p)
{
	if (isSleeping)
		return -1;

	LOG(LogLevel::Depricated) << "VirtualMeteoPiano::Stop() : release key." << int(p);
	/* 先檢查是否可以輸入，可以的時候才能控制 */
	if (!isActive)
		return 0;

	if (sustainType == VirtualMeteoPianoSustainType::Auto)
		return 0;

	//if (!dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(p)))
	//	return 0;

	LOG(LogLevel::Depricated) << "VirtualMeteoPiano::Stop() : release key check pedal down." << (pedalDown?1:0);
	if (!pedalDown)
		getSamplesByPitch()->at(p)->FadeOut();
	LOG(LogLevel::Fine) << "VirtualMeteoPiano::Stop() : release key seccuess? " << (pedalDown ? 1 : 0) << "[" << int(p) << "].";

	isPressingMapByPitch[p] = false;

	return 0;
}

int VirtualMeteoPiano::PressPedal()
{
	LOG(LogLevel::Depricated) << "VirtualMeteoPiano::PressPedal() : sustain type [" << (int)sustainType << "].";
	/* 先檢查是否可以輸入，可以的時候才能控制 */
	if (!isActive)
		return 0;

	if (sustainType != VirtualMeteoPianoSustainType::Pedal)
		return 0;

	pedalDown = true;


	return 0;
}

int VirtualMeteoPiano::ReleasePedal()
{
	LOG(LogLevel::Depricated) << "VirtualMeteoPiano::ReleasePedal() : sustain type [" << (int)sustainType << "] releasing pedal.";
	/* 先檢查是否可以輸入，可以的時候才能控制 */
	if (!isActive)
		return 0;

	if (sustainType != VirtualMeteoPianoSustainType::Pedal)
		return 0;

	pedalDown = false;

	LOG(LogLevel::Fine) << "VirtualMeteoPiano::ReleasePedal() : fadeout every key.";
	map<Pitch, bool>::iterator it;
	for (it = isPressingMapByPitch.begin(); it != isPressingMapByPitch.end(); ++it) {
		if (!it->second) {
			LOG(LogLevel::Depricated) << "VirtualMeteoPiano::ReleasePedal() : fadeout [" << (int)it->first << "].";
			if (getSamplesByPitch()->find(it->first) != getSamplesByPitch()->end()) {

				//MultiPlaybackSampleChannel* sampleChannel = dynamic_cast<MultiPlaybackSampleChannel*>(getSamplesByPitch()->at(it->first));
				//if (sampleChannel) {
				//	if (sampleChannel->GetIsPlaying(1))
				//		sampleChannel->FadeOut(1);
				//}

				LOG(LogLevel::Fine) << "VirtualMeteoPiano::ReleasePedal() : fadeout [" << (int)it->first << "] success? " << (getSamplesByPitch()->at(it->first)->GetIsPlaying() ? 1 : 0);
				if(getSamplesByPitch()->at(it->first)->GetIsPlaying())
					getSamplesByPitch()->at(it->first)->FadeOut();

			}
		}
	}


	return 0;
}

int VirtualMeteoPiano::loadAndMapPitches()
{
	// 在VirtualMeteoPiano中沒有輸入，所以不用管piano action怎麼對應到pitch
	return 0;
}

int VirtualMeteoPiano::resetState()
{
	map<Pitch, bool>::iterator it;
	for (it = isPressingMapByPitch.begin(); it != isPressingMapByPitch.end(); ++it) {
		(*it).second = false;
	}

	map<Pitch, SampleChannel*>::iterator it2;
	for (it2 = getSamplesByPitch()->begin(); it2 != getSamplesByPitch()->end(); ++it2) {
		(*it2).second->Stop();
	}

	return 0;
}

int VirtualMeteoPiano::MoveOctave(PianoPitchMovement m)
{
	// 在這邊沒用
	throw runtime_error("VirtualMeteoPiano::MoveOctave() : this function is not available in VirtualMeteoPiano.");
	return 0;
}

int VirtualMeteoPiano::SwitchSoundBindings(TSoundBindingSet<Pitch>* sBindingSet)
{
	// 這樣寫錯了
	//LOG(LogLevel::Debug) << "VirtualMeteoPiano::SwitchSoundBindings() : switch piano sound to [" << sBindingSet->fileName << "].";
	//
	//Piano::SwitchSoundBindings(sBindingSet);
	//LOG(LogLevel::Debug) << [=]() {
	//
	//	map<Pitch, SampleChannel*>::iterator it;
	//	for (it = getSamplesByPitch()->begin(); it != getSamplesByPitch()->end(); ++it) {
	//
	//		LOG(LogLevel::Debug) << "VirtualMeteoPiano::SwitchSoundBindings() : switch to [" << (int)(*it).first << "]";
	//	}
	//
	//	return 0;
	//}();
	//
	//return 0;

	LOG(LogLevel::Debug) << "VirtualMeteoPiano::SwitchSoundBindings() : switch piano sound to [" << sBindingSet->fileName << "].";

	if (sBindingSet == soundBindingSet) {

		// output manager -> 藍芽回傳這是一樣的音色
		return 0;
	}
	soundBindingSet = sBindingSet;


	/* 暫停任何輸入 */
	isActive == false;

	/* 重置鋼琴狀態 */
	resetState();

	/* 暫停sample manager更新 */
	audioManager->GetMirrorSampleManager()->SetIsActive(false);

	/* 更新sound binding */
	for (int i = 0; i < soundBindings.size(); i++) {

		if (audioManager->GetMirrorSampleManager()->HasSampleChannel(soundBindings[i]))
			audioManager->GetMirrorSampleManager()->RemoveSampleChannel(soundBindings[i]);
		delete soundBindings[i];
	}
	soundBindings.clear();

	for (int i = (int)soundBindingSet->startKey; i <= (int)soundBindingSet->endKey; i++) {
		soundBindings.push_back(soundBindingSet->GetSoundBinding(Pitch(i)));
	}

	/* 重置smaple channel */
	//audioManager->GetSampleManager()->ClearSampleChannels();	// 不應該直接清空，有些sample可能不是樂器的，是其他音效

	loadAndMapSamples();

	/* 重啟sample manager更新 */
	audioManager->GetMirrorSampleManager()->SetIsActive(true);

	/* 重啟任何輸入 */
	isActive == true;

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
	
	samplesByPitch[Pitch::C1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::C1 ));
	samplesByPitch[Pitch::D1b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::D1b));
	samplesByPitch[Pitch::D1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::D1 ));
	samplesByPitch[Pitch::E1b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::E1b));
	samplesByPitch[Pitch::E1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::E1 ));
	samplesByPitch[Pitch::F1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::F1 ));
	samplesByPitch[Pitch::G1b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::G1b));
	samplesByPitch[Pitch::G1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::G1 ));
	samplesByPitch[Pitch::A1b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::A1b));
	samplesByPitch[Pitch::A1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::A1 ));
	samplesByPitch[Pitch::B1b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::B1b));
	samplesByPitch[Pitch::B1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::B1 ));
																   
	samplesByPitch[Pitch::C  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::C  ));
	samplesByPitch[Pitch::Db ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Db ));
	samplesByPitch[Pitch::D  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::D  ));
	samplesByPitch[Pitch::Eb ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Eb ));
	samplesByPitch[Pitch::E  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::E  ));
	samplesByPitch[Pitch::F  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::F  ));
	samplesByPitch[Pitch::Gb ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Gb ));
	samplesByPitch[Pitch::G  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::G  ));
	samplesByPitch[Pitch::Ab ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Ab ));
	samplesByPitch[Pitch::A  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::A  ));
	samplesByPitch[Pitch::Bb ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::Bb ));
	samplesByPitch[Pitch::B  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::B  ));
																   	  
	samplesByPitch[Pitch::c  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c  ));
	samplesByPitch[Pitch::db ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::db ));
	samplesByPitch[Pitch::d  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d  ));
	samplesByPitch[Pitch::eb ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::eb ));
	samplesByPitch[Pitch::e  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e  ));
	samplesByPitch[Pitch::f  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f  ));
	samplesByPitch[Pitch::gb ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::gb ));
	samplesByPitch[Pitch::g  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g  ));
	samplesByPitch[Pitch::ab ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::ab ));
	samplesByPitch[Pitch::a  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a  ));
	samplesByPitch[Pitch::bb ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::bb ));
	samplesByPitch[Pitch::b  ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b  ));
		   														   	  
	samplesByPitch[Pitch::c1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c1 ));
	samplesByPitch[Pitch::d1b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d1b));
	samplesByPitch[Pitch::d1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d1 ));
	samplesByPitch[Pitch::e1b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e1b));
	samplesByPitch[Pitch::e1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e1 ));
	samplesByPitch[Pitch::f1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f1 ));
	samplesByPitch[Pitch::g1b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g1b));
	samplesByPitch[Pitch::g1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g1 ));
	samplesByPitch[Pitch::a1b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a1b));
	samplesByPitch[Pitch::a1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a1 ));
	samplesByPitch[Pitch::b1b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b1b));
	samplesByPitch[Pitch::b1 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b1 ));
		   														   	  
	samplesByPitch[Pitch::c2 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c2 ));
	samplesByPitch[Pitch::d2b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d2b));
	samplesByPitch[Pitch::d2 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d2 ));
	samplesByPitch[Pitch::e2b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e2b));
	samplesByPitch[Pitch::e2 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e2 ));
	samplesByPitch[Pitch::f2 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f2 ));
	samplesByPitch[Pitch::g2b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g2b));
	samplesByPitch[Pitch::g2 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g2 ));
	samplesByPitch[Pitch::a2b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a2b));
	samplesByPitch[Pitch::a2 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a2 ));
	samplesByPitch[Pitch::b2b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b2b));
	samplesByPitch[Pitch::b2 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b2 ));
																   	  
	samplesByPitch[Pitch::c3 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::c3 ));
	samplesByPitch[Pitch::d3b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d3b));
	samplesByPitch[Pitch::d3 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::d3 ));
	samplesByPitch[Pitch::e3b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e3b));
	samplesByPitch[Pitch::e3 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::e3 ));
	samplesByPitch[Pitch::f3 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::f3 ));
	samplesByPitch[Pitch::g3b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g3b));
	samplesByPitch[Pitch::g3 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::g3 ));
	samplesByPitch[Pitch::a3b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a3b));
	samplesByPitch[Pitch::a3 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::a3 ));
	samplesByPitch[Pitch::b3b] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b3b));
	samplesByPitch[Pitch::b3 ] = audioManager->GetMirrorSampleManager()->GetSampleChannel(getSoundBinding((int)Pitch::b3 ));


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
