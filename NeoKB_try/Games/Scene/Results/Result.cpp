#include "Result.h"


using namespace Games::Scenes::Results;


Result::Result(Score * s): RegisterType("Result")
{
	score = s;
}

int Result::onEntering(Scene * lastScene)
{
	MeteoScene::onEntering(lastScene);

	return 0;
}

int Result::onExiting(Scene * lastScene)
{
	MeteoScene::onExiting(lastScene);
	if (score != nullptr)
		delete score;

	return 0;
}
