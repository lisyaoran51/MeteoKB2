#ifndef SAMPLE_CHANNEL_GENERATOR_H
#define SAMPLE_CHANNEL_GENERATOR_H


#include "../../Allocation/Hierachal/MtoObject.h"
#include "SampleChannel.h"
#include "../../../Instruments/Audio/SoundBinding.h"
#include "../../IO/Stores/CompositeResourceStore.h"


using namespace Framework::Allocation::Hierachal;
using namespace Instruments::Audio;
using namespace Framework::IO::Stores;


namespace Framework {
namespace Audio {
namespace Samples {

	class SampleChannelGenerator : public MtoObject {

	public:

		SampleChannelGenerator();

		SampleChannelGenerator(CompositeResourceStore<char*>* rStore);

		virtual int SetResourceStore(CompositeResourceStore<char*>* rStore);

		virtual SampleChannel* GenerateSampleChannel(SoundBinding* soundBinding) = 0;

	protected:

		CompositeResourceStore<char*>* resourceStore = nullptr;


	};

}}}


#endif