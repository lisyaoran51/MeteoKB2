#ifndef MANUAL_PEDAL_MODIFIER_H
#define MANUAL_PEDAL_MODIFIER_H



#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Instruments/Instrument.h"
#include "../../../Games/Ruleset/Modifiers/InstrumentModifier.h"
#include "../../../Games/Ruleset/Modifiers/EventProcessorFilterModifier.h"



using namespace Games::Rulesets::Modifiers;
using namespace Instruments;


namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	/// <summary>
	/// 這邊還不能寫，要等照譜播音樂那邊寫完才能寫，不然不知道會不會filter掉原曲的音
	/// </summary>
	class ManualPedalModifier : public InstrumentModifier, public EventProcessorFilterModifier {

	public:

		ManualPedalModifier();

		virtual int ApplyToInstrument(Instrument* instrument);

		virtual int ApplyToEventProcessorFilter(EventProcessorFilter* eventProcessorFilter);

	protected:

		int filterAutoPedalEventProcessors(vector<EventProcessor<Event>*>* eventProcessors);

	};

}}}




#endif