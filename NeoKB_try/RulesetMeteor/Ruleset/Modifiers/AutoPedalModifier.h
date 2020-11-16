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

	/// <summary>
	/// 這個應該要拿掉，以後只要譜裡有pedal就要自動踏踏板，不用用這個modifier去設定
	///	</summary>
	class AutoPedalModifier : public InstrumentModifier {

	public:

		AutoPedalModifier();

		virtual int ApplyToInstrument(Instrument* instrument);


	};

}}}




#endif