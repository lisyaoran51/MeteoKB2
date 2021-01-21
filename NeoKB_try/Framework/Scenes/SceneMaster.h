#ifndef SCENE_MASTER_H
#define SCENE_MASTER_H


#include <map>
#include "../../Util/Singleton.h"
#include "Scene.h"


using namespace std;
using namespace Util;


namespace Framework {
namespace Scenes {

	

	class SceneMaster : public Singleton<SceneMaster> {

	public:

		SceneMaster() = default;

		int AddScene(Scene* s);

		int DeleteScene(Scene* s);
		int DeleteScene(string sceneName);

		bool CheckScene(Scene* s);
		bool CheckScene(string sceneName);

	protected:

		map<string, Scene*> scenes;

	};

}
}



#endif