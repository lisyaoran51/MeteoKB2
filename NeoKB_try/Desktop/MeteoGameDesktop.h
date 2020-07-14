#ifndef METEO_GAME_DESKTOP_H
#define METEO_GAME_DESKTOP_H

#include "../Games/MeteoGame.h"
#include "../Framework/IO/PlatformStorage.h"


using namespace Games;
using namespace Framework::IO;



namespace Desktop {

	/// <summary>
	/// 
	/// </summary>
	class MeteoGameDesktop : public MeteoGame {

	public:

		MeteoGameDesktop(vector<string>& args);

		/// <summary>
		/// 固定的檔案，這邊是不能寫入的，是放一些最重要的檔案
		/// 之後要更新的檔案應該要再切一個磁碟分割出來，應該不會在這
		/// 之後其他的分割，例如tmpfs和可寫入的分割，要從這邊建一個link連過去
		/// 這樣就可以當作是在同一個資料夾下，但其實是在不同分割下
		/// </summary>
		virtual Storage* GetStableStorage();

	private:

		class StableStorage : public PlatformStorage {

			string rootName = "";

		public:

			StableStorage(string basePath);

		protected:

			virtual string locateBasePath();

		};

	};

}




#endif