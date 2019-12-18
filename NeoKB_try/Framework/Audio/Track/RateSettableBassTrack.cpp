#include "RateSettableBassTrack.h"

#include "../../../ThirdParty/Bass/bass_fx.h"


using namespace Framework::Audio::Tracks;


RateSettableBassTrack::RateSettableBassTrack(char * fileName): BassTrack()
{
	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {


		sourceStream = BASS_StreamCreateFile(false, fileName, 0, 0, 0);

		stream = BASS_FX_TempoCreate(sourceStream, BASS_FX_FREESOURCE);

		//Length = Bass.ChannelBytes2Seconds(activeStream, Bass.ChannelGetLength(activeStream)) * 1000;
		length = BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetLength(stream, BASS_POS_BYTE));
		LOG(LogLevel::Info) << "RateSettableBassTrack::RateSettableBassTrack() : create stream [" << (stream != 0 ? 0 : BASS_ErrorGetCode()) << "] in path [" << fileName << "]. length = [" << length << "].";

		return 0;
	}, "Lambda_RateSettableBassTrack::CreateStream");
}

int RateSettableBassTrack::SetRate(double r)
{

	float frequency = 44100;
	BASS_ChannelGetAttribute(BASS_FX_TempoGetSource(stream), BASS_ATTRIB_FREQ, &frequency);
	frequency = frequency * rate / r;
	BASS_ChannelSetAttribute(stream, BASS_ATTRIB_TEMPO_FREQ, frequency);

	float pitch = 0;
	BASS_ChannelGetAttribute(BASS_FX_TempoGetSource(stream), BASS_ATTRIB_TEMPO_PITCH, &pitch);
	pitch = pitch - log(rate / r) / logFrequencyToPitch; // / log(1.0594630943592953098f);
	BASS_ChannelSetAttribute(stream, BASS_ATTRIB_TEMPO_PITCH, pitch);

	rate = r;
	return 0;
}
