#include "MultiPlaybackSampleChannel.h"


using namespace Framework::Audio::Samples;


MultiPlaybackSampleChannel::MultiPlaybackSampleChannel(Sample * s, int pAmount, int tAmount, OverrideType oType): SampleChannel(s)
{

	playbackAmount = pAmount;
	trackAmount = tAmount;

	//channelIds = new int[pAmount][tAmount];
	void **p;


	channelIds = new int*[tAmount];
	for (int i = 0; i < tAmount; i++)
		channelIds[i] = new int[pAmount];

	isTrackFadingOut = new bool[tAmount];
	for (int i = 0; i < tAmount; i++)
		isTrackFadingOut[i] = false;

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		for (int i = 0; i < tAmount; i++) {

			for (int j = 0; j < pAmount; j++) {
				channelIds[i][j] = createSampleChannel();
			}
			

		}

		return 0;
	}, "Lambda_MultiPlaybackSampleChannel::CreateSampleChannels");

	

}

int MultiPlaybackSampleChannel::Play(double v)
{
	SampleChannel::Play(v);

	PlayOnTrack(0, v);
	return 0;
}
