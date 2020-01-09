#ifndef SAMPLE_CHANNEL_H
#define SAMPLE_CHANNEL_H

#include "../AudioCollectionManager.h"
#include "Sample.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class SampleChannel : public AdjustableAudioComponent {


	public:

		SampleChannel();

		SampleChannel* GetSampleChannel(string name);

		virtual int Play();

		virtual int Play(double volume);

		virtual int Stop();

	protected:

		double initialVolume = 0;

		SampleChannel(Sample* s);

		Sample* sample;

		bool isPlaying;

		bool isStarted;

		bool isCompleted;

	};


}}}





#endif