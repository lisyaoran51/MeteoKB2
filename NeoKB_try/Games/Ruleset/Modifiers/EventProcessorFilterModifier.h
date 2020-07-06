#ifndef EVENT_PROCESSOR_FILTER_MODIFIER_H
#define EVENT_PROCESSOR_FILTER_MODIFIER_H


#include "Modifier.h"
#include "../../Scheduler/Event/EventProcessorFilter.h"


using namespace Games::Schedulers::Events;


namespace Games {
namespace Rulesets{
namespace Modifiers{

	/// <summary>
	/// An interface for mods that make general adjustments to difficulty¡B¥ª¥k¤â.
	/// </summary>
	class EventProcessorFilterModifier : virtual public Modifier {

	public:

		virtual int ApplyToEventProcessorFilter(EventProcessorFilter* eventProcessorFilter) = 0;

	};


}}}





#endif