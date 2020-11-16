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
	/// �o�����ӭn�����A�H��u�n�и̦�pedal�N�n�۰ʽ��O�A���Υγo��modifier�h�]�w
	///	</summary>
	class AutoPedalModifier : public InstrumentModifier {

	public:

		AutoPedalModifier();

		virtual int ApplyToInstrument(Instrument* instrument);


	};

}}}




#endif