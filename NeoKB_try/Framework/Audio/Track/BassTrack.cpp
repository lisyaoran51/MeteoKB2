#include "BassTrack.h"

#include <algorithm>


using namespace Framework::Audio::Tracks;
using namespace std;


BassTrack::BassTrack(char * fileName)
{
	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {


		stream = BASS_StreamCreateFile(false, fileName, 0, 0, 0);

		//Length = Bass.ChannelBytes2Seconds(activeStream, Bass.ChannelGetLength(activeStream)) * 1000;
		length = BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetLength(stream, BASS_POS_BYTE));
		LOG(LogLevel::Info) << "BassTrack::BassTrack() : create stream [" << (stream != 0 ? 0 : BASS_ErrorGetCode()) << "] in path [" << fileName << "]. length = [" << length << "].";

		return 0;
	}, "Lambda_BassTrack::CreateStream");

}

BassTrack::BassTrack() {

}

int BassTrack::Start()
{
	Track::Start();
	LOG(LogLevel::Info) << "BassTrack::Start() : start track[" << this << "]. panding actions = [" << pendingActions.GetSize() << "].";

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		if (BASS_ChannelPlay(stream, false)) {
			isPlayed = true;
			LOG(LogLevel::Debug) << "BassTrack::Start() : play bass channel.";
		}
		else {
			isRunning = false;
			LOG(LogLevel::Debug) << "BassTrack::Start() : play bass channel failed.";

		}

		return 0;
	}, "Lambda_BassTrack::Start");

	return 0;
}

int BassTrack::Stop()
{
	Track::Stop();
	isRunning = false;

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Debug) << "BassTrack::Stop() : stop.";

		BASS_ChannelPause(stream);
		isPlayed = false;


		return 0;
	}, "Lambda_BassTrack::Stop");


	return 0;
}

bool BassTrack::Seek(double position)
{
	LOG(LogLevel::Debug) << "BassTrack::Seek : get framed source playing = [" << isPlayed << "], length = [" << length << "].";
	// TODO: 如果在還沒有start時就seek會出現錯誤，正確做法是還是要讓他seek，正確做法在osu裡面


	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		double clampedPosition = position > length ? length : position;

		if (clampedPosition != GetCurrentTime()) {

			QWORD pos = BASS_ChannelSeconds2Bytes(stream, clampedPosition);
			bool result = BASS_ChannelSetPosition(stream, pos, BASS_POS_BYTE);
			LOG(LogLevel::Debug) << "BassTrack::Seek : pending state seek time to [" << clampedPosition << "] success? " << result << ".";
		}

		return 0;
	}, "Lambda_BassTrack::Seek");
	uLock.unlock();
	
	return position <= length && position >= 0;
}

int BassTrack::Reset()
{
	return 0;
}

int BassTrack::Dispose()
{
	return 0;
}

int BassTrack::Update()
{
	Track::Update();

	isRunning = BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING;

	double currentTimeLocal = BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetPosition(stream, BASS_POS_BYTE));

	// TODO: 要讓current time不被中斷thread safe

	unique_lock<mutex> uLock(currentTimeMutex);
	currentTime = currentTimeLocal;
	uLock.unlock();

	LOG(LogLevel::Finest) << "BassTrack::Update() : get music time = [" << currentTimeLocal <<"].";


	return 0;
}
