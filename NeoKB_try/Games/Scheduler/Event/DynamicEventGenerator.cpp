#include "DynamicEventGenerator.h"

#include "../../Scene/Play/Playfield.h"


using namespace Games::Schedulers::Events;
using namespace Games::Scenes::Play;



int DynamicEventGenerator::load()
{
	Playfield* p = GetCache<Playfield>("Playfield");

	if (!p)
		throw runtime_error("int DynamicEventGenerator::load() : Playfield not found in cache.");

	return load(p);
}

int DynamicEventGenerator::load(Playfield * p)
{
	playfield = p;
	return 0;
}

DynamicEventGenerator::DynamicEventGenerator() : RegisterType("DynamicEventGenerator")
{
	// 這一個函示不用了，把playfield給cache的話遊戲結束的時候很麻煩
	throw runtime_error("DynamicEventGenerator::DynamicEventGenerator() : con't use this constructor. playfield is not in cache.");
	registerLoad(bind((int(DynamicEventGenerator::*)())&DynamicEventGenerator::load, this));

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
		playfield->AddDynamic(dynamicEvents[i]);
	}

	dynamicEvents.clear();

	return 0;
}
