#ifndef BASS_SAMPLE_H
#define BASS_SAMPLE_H

#include "Sample.h"
#include "../../../ThirdParty/Bass/bass.h"


namespace Framework {
namespace Audio {
namespace Sample {

	class BassSample : public Sample {

		int sampleID;

	public:

		BassSample(char* fileName);

		int CreateChannel();

	protected:

	};


}}}





#endif