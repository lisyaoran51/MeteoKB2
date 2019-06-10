#ifndef FRAMEWORK_CONFIG_MANAGER_H
#define FRAMEWORK_CONFIG_MANAGER_H

#include"ConfigManager.h"

using namespace Util;
using namespace std;


namespace Base {
namespace Config{

	enum class FrameworkSetting;

	class FrameworkConfigManager: public ConfigManager<FrameworkSetting> {

	public:

		FrameworkConfigManager();

		virtual ~FrameworkConfigManager() = default;

	protected:

		int InitializeDefault();

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

		CursorSensitivity,

		PatternGenerator,
		//Renderer,//?
		//HardwareMapAlgorithm,//?
		LedDriver,
		HardwareVersion

	};

}}



#endif