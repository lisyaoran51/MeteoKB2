#ifndef MULTI_PLAYBACK_BASS_SAMPLE_CHANNEL_H
#define MULTI_PLAYBACK_BASS_SAMPLE_CHANNEL_H

#include "MultiPlaybackSampleChannel.h"
#include "../../../ThirdParty/Bass/bass.h"


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