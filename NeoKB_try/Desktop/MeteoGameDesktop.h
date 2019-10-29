#ifndef METEO_GAME_DESKTOP_H
#define METEO_GAME_DESKTOP_H

#include "../Games/MeteoGame.h"
#include "../Framework/IO/PlatformStorage.h"


using namespace Games;
using namespace Framework::IO;



namespace Desktop {

	class MeteoGameDesktop : public MeteoGame {

	public:

		MeteoGameDesktop(vector<string>& args);

		virtual Storage* GetStableStorage();



	private:

		class StableStorage : public PlatformStorage {

		public:

			StableStorage();

		protected:

			virtual string locateBasePath();

		};

	};

}




#endif