#include "SceneMaster.h"

using namespace  Framework::Scenes;



int SceneMaster::AddScene(Scene * s)
{
	scenes[s->GetTypeName()] = s;
	return 0;
}

int SceneMaster::DeleteScene(Scene * s)
{
	map<string, Scene*>::const_iterator it_delete;
	for (map<string, Scene*>::const_iterator it = scenes.begin(); it != scenes.end(); ++it)
	{
		if (it->second == s) {
			it_delete = it;
			it--;
			scenes.erase(it_delete);
			return 0;
		}
	}
	return -1;
}

int SceneMaster::DeleteScene(string sceneName)
{

	map<string, Scene*>::iterator it = scenes.find(sceneName);
	if (it != scenes.end()) {
		scenes.erase(it);
		return 0;
	}

	return -1;
}

bool SceneMaster::CheckScene(Scene * s)
{
	map<string, Scene*>::const_iterator it_delete;
	for (map<string, Scene*>::const_iterator it = scenes.begin(); it != scenes.end(); ++it)
	{
		if (it->second == s) {
			return true;
		}
	}
	return false;
}

bool SceneMaster::CheckScene(string sceneName)
{
	map<string, Scene*>::iterator it = scenes.find(sceneName);
	if (it != scenes.end()) {
		return true;
	}

	return false;
}
