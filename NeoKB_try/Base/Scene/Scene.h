#ifndef SCENE_H
#define SCENE_H


#include "../../Framework/Allocation/Hierachal/Triggerable.h"



using namespace Framework::Allocation::Hierachal;



namespace Game {
namespace Scene {


	class Scene : public Triggerable, public Drawable {

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