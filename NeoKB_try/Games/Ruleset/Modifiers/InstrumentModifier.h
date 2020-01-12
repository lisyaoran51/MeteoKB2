#ifndef INSTRUMENT_MODIFIER_H
#define INSTRUMENT_MODIFIER_H


#include "../../../Instruments/Instrument.h"
#include "Modifier.h"

using namespace Instruments;


namespace Games {
namespace Rulesets{
namespace Modifiers{

	
	class InstrumentModifier: virtual public Modifier {

	public:

		virtual int ApplyToInstrument(Instrument* instrument) = 0;

	};

}}}


#endif