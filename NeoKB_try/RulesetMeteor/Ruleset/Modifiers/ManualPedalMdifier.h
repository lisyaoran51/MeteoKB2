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
	/// �o���٤���g�A�n�����м����֨���g���~��g�A���M�����D�|���|filter���즱����
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