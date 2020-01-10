#ifndef MULTI_PLAYBACK_BASS_SAMPLE_CHANNEL_H
#define MULTI_PLAYBACK_BASS_SAMPLE_CHANNEL_H

#include "MultiPlaybackSampleChannel.h"
#include "../../../ThirdParty/Bass/bass.h"


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

	class MultiPlaybackBassSampleChannel : public MultiPlaybackSampleChannel {

	public:

		MultiPlaybackBassSampleChannel(Sample* s, int pAmount, OverrideType oType);

		virtual int Play();

		virtual int Play(double v);

		virtual int Stop();

		virtual int FadeOut();

		virtual int StopFadeOut();

		virtual bool GetIsLoaded();

		virtual int OnStateChange();

	protected:

		OverrideType overrideType = OverrideType::MinimunVolume;

		virtual int createSampleChannel();

		virtual int getChannelToPlay();

	};


}}}



#endif