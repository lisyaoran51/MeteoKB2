#ifndef METEO_GAME_HOST_H
#define METEO_GAME_HOST_H


#include "../Host/GameHost.h"


using namespace Framework::Host;


namespace Framework {
namespace Host {

	class MeteoGameHost : public GameHost {

	public:
		MeteoGameHost();

	protected:

		virtual int setupMainInterface();

		virtual int SetupOutputManager(OutputManager* oManager);

		virtual vector<InputHandler*>* createAvailableInputHandlers();

		virtual Storage* getStorage(string sName);
	};


}}



#endif