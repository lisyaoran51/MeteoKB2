#ifndef CONFIGURATION_MODIFIER_H
#define CONFIGURATION_MODIFIER_H


#include "../../../Framework/Configurations/ConfigManager.h"
#include "Modifier.h"


using namespace Framework::Configurations;

namespace Games {
namespace Rulesets{
namespace Modifiers{


	class ConfigurationModifier : virtual public Modifier {

	public:

		ConfigurationModifier();

		virtual int ApplyToConfigurationManager(ConfigManager* configurationManager) = 0;

		virtual string GetConfigManagerName() = 0;

	};


	template<typename T>
	class TConfigurationModifier : public ConfigurationModifier {

	public:

		TConfigurationModifier(): RegisterType("TConfigurationModifier") {

		}

		virtual int ApplyToConfigurationManager(ConfigManager* configurationManager) {
			if (configurationManager == nullptr) {
				LOG(LogLevel::Error) << "TConfigurationModifier::ApplyToConfigurationManager() : null input.";
				return -1;
			}
				

			if (dynamic_cast<T*>(configurationManager) == nullptr) {
				LOG(LogLevel::Error) << "TConfigurationModifier::ApplyToConfigurationManager() : unable to cast the config manager to [" << GetConfigManagerName() << "].";
			}
			else
				implementApplyToConfigurationManager(dynamic_cast<T*>(configurationManager));
			return 0;
		}

	protected:

		virtual int implementApplyToConfigurationManager(T* configManager) = 0;
		

	};

}
}}






#endif