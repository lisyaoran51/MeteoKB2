#ifndef BASE_H
#define BASE_H


#include "Allocation/Hierachal/Container.h"
#include "IO/Stores/CompositeResourceStore.h"
#include "Audio/AudioManager.h"
#include "Host/GameHost.h"
#include "Output/OutputManager.h"


using namespace Framework::Allocation;
using namespace Framework::IO;
using namespace Framework::Audio;
using namespace Framework::Host;
using namespace Framework::Output;


namespace Framework {

	class Base : public Container {


		int load();

	public:

		Base();

		virtual int SetHost(GameHost* host);

		virtual string GetResourcePath();

	protected:

		AudioManager* audioManager = nullptr;

		OutputManager* outputManager = nullptr;

		GameHost* gameHost;

		CompositeResourceStore<char*>* resources;

		virtual int setupOutputManager();

	};

}


#endif