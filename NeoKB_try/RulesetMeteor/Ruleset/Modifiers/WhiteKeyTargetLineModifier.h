#ifndef WHITE_KEY_TARGET_LINE_MODIFIER_H
#define WHITE_KEY_TARGET_LINE_MODIFIER_H




#include "../../../Games/Ruleset/Modifiers/ConfigurationModifier.h"
#include "../../Config/MeteorConfigManager.h"


using namespace Games::Rulesets::Modifiers;
using namespace Meteor::Config;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class WhiteKeyTargetLineModifier : public TConfigurationModifier<MeteorConfigManager> {

	public:

		WhiteKeyTargetLineModifier();

		virtual string GetConfigManagerName();

		virtual int SetValue(float value1, float value2);

	protected:

		/// <summary>
		/// �p�G�O�b���䩳���A�N�O15�A�p�G�O�P����P���A�N�O10
		/// </summary>
		int whiteKeyTargetHeight = 15;

		virtual int implementApplyToConfigurationManager(MeteorConfigManager* configManager);

	};



}}}



#endif