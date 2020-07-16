#ifndef MULTI_PLAYBACK_SAMPLE_CHANNEL_H
#define MULTI_PLAYBACK_SAMPLE_CHANNEL_H

#include "SampleChannel.h"


namespace Framework {
namespace Audio {
namespace Samples {

	/// <summary>
	/// 當所有playback都正在play時，要播新的音效，選擇抓哪一個playback停掉來播新音效的方式
	/// </summary>
	enum class OverrideType {
		MinimunVolume,
		Longest,
	};

	/// <summary>
	/// 這邊的track是指sample會分為數個軌，進行play、stop、fadeout等動作時，只能針對一個軌
	/// 跟track manager的track不一樣，那邊的track是歌曲音檔的意思
	/// </summary>
	class MultiPlaybackSampleChannel : virtual public SampleChannel {

	public:

		
		MultiPlaybackSampleChannel(Sample* s, int pAmount, int tAmount, OverrideType oType);

		virtual int Play(double v);

		virtual int PlayOnTrack(int trackNumber) = 0;

		virtual int PlayOnTrack(int trackNumber, double v) = 0;

		virtual int Stop(int trackNumber = 0) = 0;

		//virtual int Stop(int channelId) = 0; 不會有這個狀況，要停就是所有都停，所以先不寫

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