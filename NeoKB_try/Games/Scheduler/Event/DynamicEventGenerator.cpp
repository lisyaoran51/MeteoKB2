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

	registerLoad(bind((int(DynamicEventGenerator::*)())&DynamicEventGenerator::load, this));

}

int DynamicEventGenerator::update()
{

	if (dynamicEventProcessors.size() == 0)
		return 0;

	unique_lock<mutex> uLock(dynamicEventProcessorsMutex);

	for (int i = 0; i < dynamicEventProcessors.size(); i++) {
		playfield->AddDynamic(dynamicEventProcessors[i]);
	}

	dynamicEventProcessors.clear();

	return 0;
}
