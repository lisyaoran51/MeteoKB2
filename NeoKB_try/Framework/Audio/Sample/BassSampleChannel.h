#ifndef BASS_SAMPLE_CHANNEL_H
#define BASS_SAMPLE_CHANNEL_H

#include "../AudioCollectionManager.h"
#include "SampleChannel.h"
#include "../../../ThirdParty/Bass/bass.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class BassSampleChannel : public SampleChannel {

		int channelID = -1;

	public:

		BassSampleChannel(Sample* s);

		virtual int Play();

		virtual int Play(double v);

		virtual int Stop();

		virtual int FadeOut();

		virtual int StopFadeOut();

		virtual bool GetIsPlaying();

		virtual bool GetIsLoaded();

		virtual int OnStateChange();

	protected:

		



	};


}}}





#endif