#ifndef SCENE_H
#define SCENE_H


#include "../../Framework/Allocation/Hierachal/Triggerable.h"



using namespace Framework::Allocation::Hierachal;



namespace Game {
namespace Scene {


	class Scene : public Triggerable, public Drawable {

	public:

		/// <summary>
		/// �i�J�U�@�ӵe��
		/// </summary>
		int Push(Scene* scene);

		/// <summary>
		/// �^��o�ӵe��
		/// </summary>
		int MakeCurrent();

	protected:

		Scene* parentScene;

		Scene* childScene;

		Game* game;

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