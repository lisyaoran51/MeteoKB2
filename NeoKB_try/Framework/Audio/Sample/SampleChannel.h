#ifndef SAMPLE_CHANNEL_H
#define SAMPLE_CHANNEL_H

#include "../AudioCollectionManager.h"
#include "Sample.h"


namespace Framework {
namespace Audio {
namespace Sample {

	class SampleChannel : public AdjustableAudioComponent {


	public:

		SampleChannel* GetSampleChannel(string name);

		int Play();

		int Stop();

	protected:

		SampleChannel(Sample* s);

		Sample* sample;

		bool isPlaying;

		bool isStarted;

		bool isCompleted;


	};


}}}





#endif