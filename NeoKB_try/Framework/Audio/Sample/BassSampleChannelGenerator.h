#ifndef BASS_SAMPLE_CHANNEL_GENERATOR_H
#define BASS_SAMPLE_CHANNEL_GENERATOR_H



#include "SampleChannelGenerator.h"
#include <map>


using namespace std;


namespace Framework {
namespace Audio {
namespace Samples {

	class BassSampleChannelGenerator : public SampleChannelGenerator {

	public:

		BassSampleChannelGenerator();

		BassSampleChannelGenerator(CompositeResourceStore<char*>* rStore);

		virtual SampleChannel* GenerateSampleChannel(SoundBinding* soundBinding);

		static int MoveSampleToDeleteCache(Sample* s);

		static int ClearOldSamples();

	protected:

		static map<string, Sample*> sampleCache;

		static map<string, Sample*> sampleToDelete;

	};

}}}




#endif