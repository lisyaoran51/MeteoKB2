#ifndef SCENE_H
#define SCENE_H


#include "../../Util/Hierachal/ChildAddable.h"



using namespace Util::Hierachal;



namespace Base {
namespace Scene {


	class Scene : public ChildAddable {

	public:

		/// <summary>
		/// 進入下一個畫面
		/// </summary>
		int Push(Scene* scene);

		/// <summary>
		/// 回到這個畫面
		/// </summary>
		int MakeCurrent();

	protected:

		Scene* parentScene;

		Scene* childScene;

		Game* game;

	private:

		/// <summary>
		/// 讀取game
		/// </summary>
		int load();

		/// <summary>
		/// 讀取game
		/// </summary>
		int load(Game* g);

	};

}}





#endif