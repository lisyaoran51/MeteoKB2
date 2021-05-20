#ifndef BASE_H
#define BASE_H


#include "Allocation/Hierachal/Container.h"
#include "IO/Stores/CompositeResourceStore.h"
#include "Audio/AudioManager.h"
#include "Hosts/Host.h"
#include "Output/OutputManager.h"


using namespace Framework::Allocation;
using namespace Framework::IO;
using namespace Framework::Audio;
using namespace Framework::Hosts;
using namespace Framework::Output;


namespace Framework {

	class Base : public Container {


		int load();

	public:

		Base();

		virtual int SetHost(Host* host);

		virtual string GetResourcePath();

	protected:

		AudioManager* audioManager = nullptr;

		OutputManager* outputManager = nullptr;

		Host* gameHost = nullptr;

		CompositeResourceStore<char*>* resources;

		virtual int setupOutputManager();

	};

}


#endif