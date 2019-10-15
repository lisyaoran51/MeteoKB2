#ifndef METEO_GAME_HOST_H
#define METEO_GAME_HOST_H


#include "../Host/GameHost.h"


using namespace Framework::Host;


namespace Framework {
namespace Platforms {

	class MeteoGameHost : public GameHost {

	public:
		MeteoGameHost();

	protected:

		virtual int setupMainInterface();


	};


}}



#endif