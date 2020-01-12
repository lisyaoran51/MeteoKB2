#ifndef AUTO_PEDAL_MODIFIER_H
#define AUTO_PEDAL_MODIFIER_H


#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Instruments/Instrument.h"
#include "../../../Games/Ruleset/Modifiers/InstrumentModifier.h"

using namespace Games::Rulesets::Modifiers;
using namespace Instruments;


namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class AutoPedalModifier : public InstrumentModifier {

	public:

		AutoPedalModifier();

		virtual int ApplyToInstrument(Instrument* instrument);


	};

}}}




#endif