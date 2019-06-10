#ifndef HARDWARE_INFO_H
#define HARDWARE_INFO_H

#include<string>

#define WS2812_VER_10_RENDERER "WS2812v10Renderer"
#define WS2812_VER_10_HW_MAP_ALGO "WS2812v10HwMapAlgo"
#define WS2812_VER_10_LED_DRIVER "WS2812v10LedDriver"
#define WS2812_VER_10_INPUT_MANAGER "WS2812v10InputManager"
#define WS2812_VER_10_INPUT_DRIVER "WS2812v10InputDriver"


using namespace std;

namespace Devices {
	
	struct HarewareInfo {
		int HwVersion;
		string Renderer;
		//string HwMapAlgo;
		//string LedDriver;
		string InputManager;
		string InputDriver;
	};

	static const HarewareInfo hwInfo[] = {
		{
			10,
			WS2812_VER_10_RENDERER,
			//WS2812_VER_10_HW_MAP_ALGO,
			//WS2812_VER_10_LED_DRIVER,
			WS2812_VER_10_INPUT_MANAGER,
			WS2812_VER_10_INPUT_DRIVER
		}
	};

}





#endif