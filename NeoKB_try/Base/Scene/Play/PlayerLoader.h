#ifndef PLAYER_LOADER_H
#define PLAYER_LOADER_H


#include "../Scene.h"

using namespace Base::Scene;


namespace Base {
namespace Play {

	class PlayerLoader: public Scene {

	public:


	protected:


	private:

		int load();

		/// <summary>
		/// �����߰ݬO�_Ū�������A������i�Jplayer�e��
		/// </summary>
		int pushWhenLoaded();

	};

}}






#endif
