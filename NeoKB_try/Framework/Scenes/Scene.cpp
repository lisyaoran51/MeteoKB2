#include "Scene.h"

#include "../Exceptions/FrameworkException.h"
#include "../Threading/ThreadMaster.h"
#include "SceneMaster.h"

using namespace Framework::Scenes;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Exceptions;
using namespace Framework;
using namespace Framework::Threading;




Scene::Scene() : RegisterType("Scene")
{
	isInputable = true;

	registerLoad(bind(static_cast<int(Scene::*)(void)>(&Scene::load), this));
}

int Scene::Push(Scene * scene)
{
	LOG(LogLevel::Depricated) << "Scene::Push() : Scene [" << GetTypeName() << "] push to [" << scene->GetTypeName() << "]";


	if (childScene != nullptr)
		throw FrameworkError("Scene::Push(): There Should be no child scene when pushing.");


	scene->SetParentScene(this);

	Expire();
	scene->Enter(this);

	LOG(LogLevel::Depricated) << "Scene::Push() : Scene [" << GetTypeName() << "] add [" << scene->GetTypeName() << "] to child";
	AddChild(scene);
	childScene = scene;
	LOG(LogLevel::Depricated) << "Scene::Push() : Scene [" << GetTypeName() << "] add [" << scene->GetTypeName() << "] to child over.";

	isCurrentScene = false;
	isPresent = false;

	scene->onEntered(this);

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

Scene * Scene::GetParentScene()
{
	return parentScene;
}

int Scene::SetIsValidForResume(bool value)
{
	isValidForResume = value;
	return 0;
}

int Scene::Expire()
{
	onExpire();
	Deactivate();
	return 0;
}

int Scene::Enter(Scene * lastScene)
{
	isCurrentScene = true;
	isPresent = true;
	return onEntering(lastScene);
}

int Scene::OnEntered(Scene * lastScene)
{
	return 0;
}

int Scene::Resume(Scene * sourceScene)
{
	LOG(LogLevel::Depricated) << "Scene::Resume() : Scene [" << GetTypeName() << "] try to resume [" << (isValidForResume ? "Success" : "Failed") << "]";

	DeleteChild(sourceScene);
	//delete childScene;
	ThreadMaster::GetInstance().AddObjectToDelete(sourceScene); //如果會race condition的話就要這樣改

	if(sourceScene == childScene)
		childScene = nullptr;

	if (isValidForResume) {
		isCurrentScene = true;
		isPresent = true;
		onResuming(sourceScene);
	}
	else {
		Exit();
	}

	return 0;
}

int Scene::Exit()
{
	return ExitTo(this);
}

int Scene::ExitTo(Scene * sourceScene)
{
	if (isExited)
		return 0;

	LOG(LogLevel::Depricated) << "Scene::ExitTo() : Scene [" << GetTypeName() << "] try to exit to [" << parentScene->GetTypeName() << "]";

	onExiting(parentScene);

	Expire();

	parentScene->Resume(sourceScene);

	return 0;
}

int Scene::onExpire()
{
	return 0;
}

int Scene::onEntering(Scene * lastScene)
{
	LOG(LogLevel::Depricated) << "Scene::onEntering() : add [" << GetTypeName() << "] to scene master.";
	SceneMaster::GetInstance().AddScene(this);

	// 繼承的人寫
	return 0;
}

int Scene::onEntered(Scene * lastScene)
{
	return 0;
}

int Scene::onExiting(Scene * lastScene)
{
	SceneMaster::GetInstance().DeleteScene(this);

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

	LOG(LogLevel::Debug) << "Scene::onResuming() : resuming [" << GetTypeName() << "].";
	// 繼承的人寫
	return 0;
}

int Scene::load()
{

	Game * g = GetCache<Game>("Game");
	if (!g)
		throw runtime_error("int Scene::load() : Game not found in cache.");

	FrameworkConfigManager * f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int Scene::load() : FrameworkConfigManager not found in cache.");

	return load(g, f);
}

int Scene::load(Game * g, FrameworkConfigManager* f)
{
	game = g;

	int width, height;

	if (f->Get(FrameworkSetting::Width, &width) &&
		f->Get(FrameworkSetting::Height, &height)) {
		Initialize(width, height);
	}
	else
		throw runtime_error("int Scene::load(Game*, FrameworkConfigManager*) : Width and Height not found in Setting.");


	return 0;
}
