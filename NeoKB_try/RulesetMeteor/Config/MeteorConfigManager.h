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

		FallSpeed,				// �C���ʴX��
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

		RestartSection			// �p�G�n���m�ߪ����A�i�H�b�o�̿�J��ĴX�p�`�n���s����

	};

}}



#endif