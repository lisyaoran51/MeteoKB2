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

		/// <summary>
		/// 因為sample manager有兩個，兩個會重複的new同一個sample，所有要用sample cache來避免重複的new。
		/// 另外在刪sample時，也要避免把同一個Sample刪兩次，所以要先丟到delete cache去，然後等程式執行完後再一次delete sample。
		///	</summary>
		static int MoveSampleToDeleteCache(Sample* s);

		static int ClearOldSamples();

	protected:

		static map<string, Sample*> sampleCache;

		static map<string, Sample*> sampleToDelete;

	};

}}}




#endif