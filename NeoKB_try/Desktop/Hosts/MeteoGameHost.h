#ifndef METEO_GAME_HOST_H
#define METEO_GAME_HOST_H


#include "../../Games/Hosts/GameHost.h"


using namespace Games::Hosts;


namespace Desktop {
namespace Hosts {

	class MeteoGameHost : public GameHost {

	public:
		MeteoGameHost();

	protected:

		virtual int setupMainInterface();

		virtual Storage* getStorage(string sName);
	};


}}



#endif