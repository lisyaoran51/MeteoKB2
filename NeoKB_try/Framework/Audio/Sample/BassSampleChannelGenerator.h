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
		/// �]��sample manager����ӡA��ӷ|���ƪ�new�P�@��sample�A�Ҧ��n��sample cache���קK���ƪ�new�C
		/// �t�~�b�Rsample�ɡA�]�n�קK��P�@��Sample�R�⦸�A�ҥH�n�����delete cache�h�A�M�ᵥ�{�����槹��A�@��delete sample�C
		///	</summary>
		static int MoveSampleToDeleteCache(Sample* s);

		static int ClearOldSamples();

	protected:

		static map<string, Sample*> sampleCache;

		static map<string, Sample*> sampleToDelete;

	};

}}}




#endif