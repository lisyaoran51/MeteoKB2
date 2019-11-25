#ifndef BASE_H
#define BASE_H


#include "Allocation/Hierachal/Container.h"
#include "IO/Stores/CompositeResourceStore.h"
#include "Audio/AudioManager.h"
#include "Host/GameHost.h"


using namespace Framework::Allocation;
using namespace Framework::IO;
using namespace Framework::Audio;
using namespace Framework::Host;


namespace Framework {

	class Base : public Container {


		int load();

	public:

		Base();

		virtual int SetHost(GameHost* host);

		virtual string GetResourcePath();

	protected:

		AudioManager* audioManager;

		GameHost* gameHost;

		CompositeResourceStore<char*>* resources;

	};

}


#endif