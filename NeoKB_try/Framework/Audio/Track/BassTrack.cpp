#include "BassTrack.h"

#include <algorithm>


using namespace Framework::Audio::Tracks;
using namespace std;


BassTrack::BassTrack(char * fileName)
{
	pendingActions.Add(this, [=]() {

		stream = BASS_StreamCreateFile(false, fileName, 0, 0, 0);

		//Length = Bass.ChannelBytes2Seconds(activeStream, Bass.ChannelGetLength(activeStream)) * 1000;
		length = BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetLength(stream, BASS_POS_BYTE)) * 1000;

		return 0;
	}, "Lambda_BassTrack::CreateStream");

}

int BassTrack::Start()
{
	Track::Start();

	pendingActions.Add(this, [=]() {

		if (BASS_ChannelPlay(stream, false)) {
			isPlayed = true;
		}
		else {
			isRunning = false;
		}

		return 0;
	}, "Lambda_BassTrack::Start");

	return 0;
}

int BassTrack::Stop()
{
	Track::Stop();

	pendingActions.Add(this, [=]() {

		BASS_ChannelPause(stream);
		isPlayed = false;


		return 0;
	}, "Lambda_BassTrack::Stop");

	return 0;
}

bool BassTrack::Seek(double position)
{
	
	// TODO: 如果在還沒有start時就seek會出現錯誤，正確做法是還是要讓他seek，正確做法在osu裡面

	pendingActions.Add(this, [=]() {

		double clampedPosition = position > length ? length : position;

		if (clampedPosition != GetCurrentTime()) {

			QWORD pos = BASS_ChannelSeconds2Bytes(stream, clampedPosition / (double)1000);
			BASS_ChannelSetPosition(stream, pos, BASS_POS_BYTE);

		}

		return 0;
	}, "Lambda_BassTrack::Seek");

	
	return false;
}

int BassTrack::Reset()
{
	return 0;
}

int BassTrack::Dispose()
{
	return 0;
}

int BassTrack::update()
{
	isRunning = BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING;

	double currentTimeLocal = BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetPosition(stream, BASS_POS_BYTE)) * 1000;

	// TODO: 要讓current time不被中斷thread safe
	currentTime = currentTimeLocal;

	Track::update();

	return 0;
}
