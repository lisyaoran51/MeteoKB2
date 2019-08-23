#ifndef SCENE_H
#define SCENE_H


#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../Graphic/Drawable.h"
#include "../Game.h"



using namespace Framework::Allocation::Hierachal;
using namespace Framework::Graphic;
using namespace Framework;



namespace Framework {
namespace Scenes {


	class Scene : public Container, public Drawable {

	public:

		Scene();

		/// <summary>
		/// 進入下一個畫面
		/// </summary>
		int Push(Scene* scene);

		/// <summary>
		/// 回到這個畫面
		/// </summary>
		int MakeCurrent();

		int SetParentScene(Scene* pScene);

		Scene* GetChildScene();

		int SetIsValidForResume(bool value);


		/// <summary>
		/// 卸除這個畫面
		/// </summary>
		int Expire();

		int Enter(Scene* lastScene);

		int Resume(Scene* sourceScene);

		/// <summary>
		/// 結束這個畫面。任何畫面都只會exit一次
		/// </summary>
		int Exit();

		/// <summary>
		/// 是從子場景離開，回到父場景
		/// </summary>
		int ExitTo(Scene* sourceScene);
		

	protected:

		Scene* parentScene;

		Scene* childScene;

		Game* game;

		bool isCurrentScene;

		bool isValidForResume = true;

		bool isExited = false;

		virtual int onEntering(Scene* lastScene);

		virtual int onExiting(Scene* lastScene);

		virtual int onSuspending(Scene* lastScene);

		virtual int onResuming(Scene* lastScene);

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