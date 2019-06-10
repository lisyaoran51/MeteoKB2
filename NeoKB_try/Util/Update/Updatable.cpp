#include "Updatable.h"

using namespace Util::Update;


Updatable::Updatable()
{
	currentTime = 0;
}

MTO_FLOAT Updatable::GetCurrentTime()
{
	return currentTime;
}
