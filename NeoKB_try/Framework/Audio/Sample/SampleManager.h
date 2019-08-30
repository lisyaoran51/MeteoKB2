#ifndef SAMPLE_MANAGER_H
#define SAMPLE_MANAGER_H

#include "../AudioCollectionManager.h"
#include "SampleChannel.h"
#include "../../IO/Stores/ResourceStore.h"
#include <map>

using namespace std;
using namespace Framework::IO::Stores;

namespace Framework {
namespace Audio {
namespace Samples {

	class SampleManager : public AudioCollectionManager<SampleChannel> {

	public:

		SampleManager(ResourceStore<char*>* rStore);

		SampleChannel* GetSampleChannel(string name);

	private:

		ResourceStore<char*>* resourceStore;

		map<string, Sample*> sampleCache;

	};


}}}





#endif