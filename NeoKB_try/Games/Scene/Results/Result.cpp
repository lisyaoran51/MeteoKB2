#include "Result.h"


using namespace Games::Scenes::Results;


Result::Result(Score * s): RegisterType("Result")
{
}

int Result::onEntering(Scene * lastScene)
{


	return 0;
}

int Result::onExiting(Scene * lastScene)
{
	if (score != nullptr)
		delete score;

	return 0;
}
