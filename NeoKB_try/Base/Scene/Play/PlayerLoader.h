#ifndef PLAYER_LOADER_H
#define PLAYER_LOADER_H


#include "../../../Framework/Scenes/Scene.h"

using namespace Framework::Scene;
using namespace Framework::Allocation::Hierachal;

namespace Base {
namespace Play {

	class PlayerLoader: public Scene {

	public:


	protected:


	private:

		int load();

		/// <summary>
		/// 不停詢問是否讀取完成，完成後進入player畫面
		/// </summary>
		int pushWhenLoaded();

	};

}}






#endif
