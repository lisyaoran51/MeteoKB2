#ifndef INSTANT_CONFIG_MANAGER_H
#define INSTANT_CONFIG_MANAGER_H

#include "../../Framework/Configurations/FrameworkConfigManager.h"

using namespace Util;
using namespace std;
using namespace Framework::Configurations;


namespace Instant {
namespace Config{

	enum class InstantSetting;

	class InstantConfigManager : public TConfigManager<InstantSetting> {

	public:

		InstantConfigManager();

		virtual ~InstantConfigManager() = default;

	protected:

		int InitializeDefault();

	};


	enum class InstantSetting {

		PatternGenerator,
		InstantFallMapAlgorithm,
		InstantGlowLineMapAlgorithm,
		InstantVirtualInstrumentController,
		InstantSystemController,

		WhiteKeyTargetHeight,
		BlackKeyTargetHeight,

		FallSpeed,				// ¨C¬í²¾°Ê´XÁû
		FallBrightness,
		FallLength,

	};

}}



#endif