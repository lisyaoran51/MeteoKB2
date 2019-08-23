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
		/// �i�J�U�@�ӵe��
		/// </summary>
		int Push(Scene* scene);

		/// <summary>
		/// �^��o�ӵe��
		/// </summary>
		int MakeCurrent();

		int SetParentScene(Scene* pScene);

		Scene* GetChildScene();

		int SetIsValidForResume(bool value);


		/// <summary>
		/// �����o�ӵe��
		/// </summary>
		int Expire();

		int Enter(Scene* lastScene);

		int Resume(Scene* sourceScene);

		/// <summary>
		/// �����o�ӵe���C����e�����u�|exit�@��
		/// </summary>
		int Exit();

		/// <summary>
		/// �O�q�l�������}�A�^�������
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
		/// Ū��game
		/// </summary>
		int load();

		/// <summary>
		/// Ū��game
		/// </summary>
		int load(Game* g);

	};

}}





#endif