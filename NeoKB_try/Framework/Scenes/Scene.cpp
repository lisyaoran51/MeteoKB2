#include "Scene.h"

#include "../Exceptions/FrameworkException.h"

using namespace Framework::Scenes;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Exceptions;
using namespace Framework;




Scene::Scene() : RegisterType("Scene")
{
	isInputable = true;
}

int Scene::Push(Scene * scene)
{
	if (childScene != nullptr)
		throw FrameworkError("Scene::Push(): There Should be no child scene when pushing.");


	scene->SetParentScene(this);

	AddChild(scene);
	childScene = scene;

	Expire();

	isCurrentScene = false;
	isPresent = false;
	scene->Enter(this);


	return 0;
}

int Scene::MakeCurrent()
{
	if (isCurrentScene)
		return 0;


	Scene* s = nullptr;
	for (s = childScene; s->GetChildScene() != nullptr; s = s->GetChildScene()) {
		s->SetIsValidForResume(false);
	}
	// 會直接把所有child全部exit
	s->Exit();

	isCurrentScene = true;
	isPresent = true;

	return 0;
}

int Scene::SetParentScene(Scene * pScene)
{
	parentScene = pScene;
	return 0;
}

Scene * Scene::GetChildScene()
{
	return childScene;
}

int Scene::SetIsValidForResume(bool value)
{
	isValidForResume = value;
	return 0;
}

int Scene::Expire()
{
	Deactivate();
	return 0;
}

int Scene::Enter(Scene * lastScene)
{
	isCurrentScene = true;
	isPresent = true;
	return onEntering(lastScene);
}

int Scene::Resume(Scene * sourceScene)
{
	delete childScene;
	childScene = nullptr;

	if (isValidForResume) {
		onResuming(sourceScene);
	}
	else {
		ExitTo(sourceScene);
	}

	return 0;
}

int Scene::Exit()
{
	return ExitTo(nullptr);
}

int Scene::ExitTo(Scene * sourceScene)
{
	if (isExited)
		return 0;

	onExiting(parentScene);

	Expire();

	parentScene->Resume(sourceScene);

	return 0;
}

int Scene::onEntering(Scene * lastScene)
{
	// 繼承的人寫
	return 0;
}

int Scene::onExiting(Scene * lastScene)
{
	// 繼承的人寫
	return 0;
}

int Scene::onSuspending(Scene * lastScene)
{
	// 繼承的人寫
	return 0;
}

int Scene::onResuming(Scene * lastScene)
{
	// 繼承的人寫
	return 0;
}

int Scene::load()
{
	return 0;
}

int Scene::load(Game * g)
{
	game = g;
	return 0;
}
