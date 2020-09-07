#ifndef DYNAMIC_EVENT_GENERATOR_H
#define DYNAMIC_EVENT_GENERATOR_H


#include "../../../Framework/Allocation/Hierachal/Container.h"
#include "EventProcessor.h"


using namespace Framework::Allocation::Hierachal;

namespace Games {
namespace Scenes{
namespace Play{

	class Playfield;

}}}

using namespace Games::Scenes::Play;


namespace Games {
namespace Schedulers {
namespace Events{

	class DynamicEventGenerator : public Container {

		int load();

		int load(Playfield* p);

	public:

		DynamicEventGenerator();

		DynamicEventGenerator(Playfield* p);

	protected:

		mutable mutex dynamicEventProcessorsMutex;

		Playfield* playfield = nullptr;

		vector<EventProcessor<Event>*> dynamicEventProcessors;

		virtual int update();

	};

}}}





#endif