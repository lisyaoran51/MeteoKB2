#ifndef METEOR_CONFIG_MANAGER_H
#define METEOR_CONFIG_MANAGER_H

#include "../../Framework/Configurations/FrameworkConfigManager.h"

using namespace Util;
using namespace std;
using namespace Framework::Configurations;


namespace Meteor {
namespace Config{

	enum class MeteorSetting;

	class MeteorConfigManager: public ConfigManager<MeteorSetting> {

	public:

		MeteorConfigManager();

		virtual ~MeteorConfigManager() = default;

	protected:

		int InitializeDefault();

	};


	enum class MeteorSetting {

		PatternGenerator,
		FallMapAlgorithm,
		EruptMapAlgorithm,
		ExplodeMapAlgorithm,
		GlowLineMapAlgorithm,
		TargetLineMapAlgorithm,
		SustainPedalLightRingIoCommunicator,
		InstrumentController,
		RepeatPracticeController,

		MapPitchShifter,
		OctaveShifter,
		PitchShiftSpeed,

		WhiteKeyTargetHeight,
		BlackKeyTargetHeight,

		FallSpeed,				// 每秒移動幾顆
		FallBrightness,
		FallLength,
		ExplodeSpeed,
		ExplodeBrightness,
		ExplodeLifeTime,
		GlowLineSpeed,
		GlowLineDuration,
		GlowLineBrightness,

		TargetLineBlinkSpeed,
		TargetLineBrightness,

		RestartSection			// 如果要玩練習版本，可以在這裡輸入到第幾小節要重新播放

	};

}}



#endif