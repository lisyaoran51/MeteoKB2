#ifndef SAMPLE_MANAGER_H
#define SAMPLE_MANAGER_H

#include "../AudioCollectionManager.h"
#include "SampleChannel.h"
#include "../../IO/Stores/CompositeResourceStore.h"
#include <map>

using namespace std;
using namespace Framework::IO::Stores;

namespace Framework {
namespace Audio {
namespace Samples {

	class SampleManager : public AudioCollectionManager<SampleChannel> {

	public:

		SampleManager(CompositeResourceStore<char*>* rStore);

		SampleChannel* GetSampleChannel(string name);

		SampleChannel* GetMultiPlaybackSampleChannel(string name);

		int SetPlaybackAmount(int pAmount);

	protected:

		int playbackAmount = 10;

	private:

		CompositeResourceStore<char*>* resourceStore;

		map<string, Sample*> sampleCache;

		map<string, SampleChannel*> sampleChannelCache;

	};


}}}





#endif