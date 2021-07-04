#include "DynamicEventGenerator.h"

#include "../../Scene/Play/Playfield.h"


using namespace Games::Schedulers::Events;
using namespace Games::Scenes::Play;




DynamicEventGenerator::DynamicEventGenerator() : RegisterType("DynamicEventGenerator")
{
	// 這一個函示不用了，把playfield給cache的話遊戲結束的時候很麻煩
	throw runtime_error("DynamicEventGenerator::DynamicEventGenerator() : con't use this constructor. playfield is not in cache.");

}

DynamicEventGenerator::DynamicEventGenerator(Playfield * p) : RegisterType("DynamicEventGenerator")
{
	playfield = p;
}

int DynamicEventGenerator::update()
{

	if (dynamicEvents.size() == 0)
		return 0;

	unique_lock<mutex> uLock(dynamicEventsMutex);

	for (int i = 0; i < dynamicEvents.size(); i++) {
		try {
			playfield->AddDynamic(dynamicEvents[i]);
		}
		catch (exception &e) {
			LOG(LogLevel::Warning) << "DynamicEventGenerator::update() : Fail to add new dynamic event:" << e.what();
		}
	}

	dynamicEvents.clear();

	return 0;
}
