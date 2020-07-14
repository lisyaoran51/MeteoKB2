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
		/// �T�w���ɮסA�o��O����g�J���A�O��@�ǳ̭��n���ɮ�
		/// ����n��s���ɮ����ӭn�A���@�ӺϺФ��ΥX�ӡA���Ӥ��|�b�o
		/// �����L�����ΡA�Ҧptmpfs�M�i�g�J�����ΡA�n�q�o��ؤ@��link�s�L�h
		/// �o�˴N�i�H��@�O�b�P�@�Ӹ�Ƨ��U�A�����O�b���P���ΤU
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