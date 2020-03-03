#include "RateSettableBassTrack.h"

#include "../../../ThirdParty/Bass/bass_fx.h"
#include <cmath>


using namespace Framework::Audio::Tracks;
using namespace std;



RateSettableBassTrack::RateSettableBassTrack(char * fileName): BassTrack()
{

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {


		sourceStream = BASS_StreamCreateFile(false, fileName, 0, 0, BASS_STREAM_DECODE);

		stream = BASS_FX_TempoCreate(sourceStream, BASS_FX_FREESOURCE);
		
		
			
		//Length = Bass.ChannelBytes2Seconds(activeStream, Bass.ChannelGetLength(activeStream)) * 1000;
		length = BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetLength(stream, BASS_POS_BYTE));
		LOG(LogLevel::Info) << "RateSettableBassTrack::RateSettableBassTrack() : create stream [" << (stream != 0 ? 0 : BASS_ErrorGetCode()) << "] in path [" << fileName << "]. length = [" << length << "].";

		return 0;
	}, "Lambda_RateSettableBassTrack::CreateStream");

	LOG(LogLevel::Debug) << "RateSettableBassTrack::RateSettableBassTrack() : constructor done.";
}

int RateSettableBassTrack::SetRate(double r)
{

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Debug) << "RateSettableBassTrack::SetRate() : set rate to " << r << ".";
		float frequency = 44100;
		BASS_ChannelGetAttribute(BASS_FX_TempoGetSource(stream), BASS_ATTRIB_FREQ, &frequency);
		frequency = frequency * r;//rate / r;
		BASS_ChannelSetAttribute(stream, BASS_ATTRIB_TEMPO_FREQ, frequency);

		// 不能直接調整，每次數值都要重設，bass拿不到目前的pitch
		//BASS_ChannelGetAttribute(BASS_FX_TempoGetSource(stream), BASS_ATTRIB_TEMPO_PITCH, &pitch);
		pitch = -log(r) / logFrequencyToPitch; // / log(1.0594630943592953098f);
		BASS_ChannelSetAttribute(stream, BASS_ATTRIB_TEMPO_PITCH, pitch);

		rate = r;
		return 0;
	}, "Lambda_RateSettableBassTrack::SetRate");
	
	return 0;
}
