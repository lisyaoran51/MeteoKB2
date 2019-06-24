#ifndef SCENE_H
#define SCENE_H


#include "../../Util/Hierachal/ChildAddable.h"



using namespace Util::Hierachal;



namespace Base {
namespace Scene {


	class Scene : public ChildAddable {

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