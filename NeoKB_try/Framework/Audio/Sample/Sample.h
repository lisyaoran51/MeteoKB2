#ifndef SAMPLE_H
#define SAMPLE_H

#include "../AudioCollectionManager.h"



namespace Framework {
namespace Audio {
namespace Sample {

	class Sample : public AudioComponent {


	public:

		virtual bool GetIsLoaded();

	protected:

		bool isLoaded;

	};


}}}





#endif