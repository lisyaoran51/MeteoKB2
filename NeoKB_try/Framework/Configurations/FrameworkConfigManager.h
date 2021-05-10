#ifndef FRAMEWORK_CONFIG_MANAGER_H
#define FRAMEWORK_CONFIG_MANAGER_H

#include "ConfigManager.h"

using namespace Util;
using namespace std;


namespace Framework {
namespace Configurations {

	enum class FrameworkSetting;

	class FrameworkConfigManager: public TConfigManager<FrameworkSetting> {

	public:

		FrameworkConfigManager();

		virtual ~FrameworkConfigManager() = default;

	protected:

		virtual int initializeDefault();

		virtual int setNamesOfConfig();


	};


	enum class FrameworkSetting {
		
		SongTitle, // 之後要刪掉，擺到正確的地方
		
		AudioDevice,
		VolumeUniversal,
		VolumeEffect,
		VolumeMusic,

		Width,
		Height,
		TargetHeight,
		BlackKeyHeight,
		BlackKeyTargetHeight,

		StartPitch,

		FrameSync,
		FrameRate,
		UpdateRate,
		InputRate,
		AudioUpdateRate,

		CursorSensitivity,

		PatternGenerator,
		//Renderer,//?
		//HardwareMapAlgorithm,//?
		LedDriver,
		HardwareVersion,
		PeriodMapInterval

	};

}}



#endif