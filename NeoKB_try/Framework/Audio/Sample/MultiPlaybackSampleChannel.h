#ifndef MULTI_PLAYBACK_SAMPLE_CHANNEL_H
#define MULTI_PLAYBACK_SAMPLE_CHANNEL_H

#include "SampleChannel.h"


namespace Framework {
namespace Audio {
namespace Samples {

	/// <summary>
	/// ��Ҧ�playback�����bplay�ɡA�n���s�����ġA��ܧ���@��playback�����Ӽ��s���Ī��覡
	/// </summary>
	enum class OverrideType {
		MinimunVolume,
		Longest,
	};

	/// <summary>
	/// �o�䪺track�O��sample�|�����ƭӭy�A�i��play�Bstop�Bfadeout���ʧ@�ɡA�u��w��@�ӭy
	/// ��track manager��track���@�ˡA���䪺track�O�q�����ɪ��N��
	/// </summary>
	class MultiPlaybackSampleChannel : virtual public SampleChannel {

	public:

		
		MultiPlaybackSampleChannel(Sample* s, int pAmount, int tAmount, OverrideType oType);

		virtual int Play(double v);

		virtual int PlayOnTrack(int trackNumber) = 0;

		virtual int PlayOnTrack(int trackNumber, double v) = 0;

		virtual int Stop(int trackNumber = 0) = 0;

		//virtual int Stop(int channelId) = 0; ���|���o�Ӫ��p�A�n���N�O�Ҧ������A�ҥH�����g

		virtual int FadeOut(int trackNumber = 0) = 0;

		virtual int StopFadeOut(int trackNumber = 0) = 0;

		virtual bool GetIsPlaying(int trackNumber = 0) = 0;

		virtual bool GetIsLoaded() = 0;

		virtual int OnStateChange(int trackNumber = 0) = 0;

	protected:

		int playbackAmount = 0;

		int trackAmount = 0;

		int** channelIds = nullptr;

		bool* isTrackFadingOut = nullptr;

		OverrideType overrideType = OverrideType::MinimunVolume;

		virtual int getChannelToPlay(int trackNumber = 0) = 0;

		virtual int createSampleChannel() = 0;

	};


}}}



#endif