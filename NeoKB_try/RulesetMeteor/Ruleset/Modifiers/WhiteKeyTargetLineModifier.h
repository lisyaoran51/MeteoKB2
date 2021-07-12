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
		/// 如果是在白鍵底部，就是15，如果是與黑鍵同高，就是10
		/// </summary>
		int whiteKeyTargetHeight = 15;

		virtual int implementApplyToConfigurationManager(MeteorConfigManager* configManager);

	};



}}}



#endif