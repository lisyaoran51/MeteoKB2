#ifndef BASS_SAMPLE_CHANNEL_GENERATOR_H
#define BASS_SAMPLE_CHANNEL_GENERATOR_H



#include "SampleChannelGenerator.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class BassSampleChannelGenerator : public SampleChannelGenerator {

	public:

		BassSampleChannelGenerator();

		BassSampleChannelGenerator(CompositeResourceStore<char*>* rStore);

		virtual SampleChannel* GenerateSampleChannel(SoundBinding* soundBinding);

	protected:

	};

}}}




#endif