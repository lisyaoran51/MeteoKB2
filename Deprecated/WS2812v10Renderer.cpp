#include "WS2812v10Renderer.h"

#include <cstring>


// defaults for cmdline options
#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                10
#define DMA                     10
//#define STRIP_TYPE            WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
#define STRIP_TYPE              WS2811_STRIP_GBR		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE            SK6812_STRIP_RGBW		// SK6812RGBW (NOT SK6812RGB)

using namespace Devices::Graphics::Renderers;


int WS2812v10Renderer::load()
{
	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int WS2812v10Renderer::load() : Framework Config Manager not found in cache.");
	return load(f);
}

int WS2812v10Renderer::load(FrameworkConfigManager * f)
{

	LOG(LogLevel::Info) << "WS2812v10Renderer::load(FrameworkConfigManager*) : Loading WS2812 v10 renderer.";

	if (f->Get(FrameworkSetting::Width, &width)) {}
	if (f->Get(FrameworkSetting::StartPitch, &startPitch)) {}
	if (f->Get(FrameworkSetting::Height, &height)) {}
	if (f->Get(FrameworkSetting::BlackKeyHeight, &blackKeyHeight)) {}

	
	int arraySize = 0;

	for (int i = 0; i < width; i++) {

		bool whiteKey = isWhiteKey(startPitch + i);

		arraySize += whiteKey ? height : blackKeyHeight;

	}

	LOG(LogLevel::Info) << "WS2812v10Renderer::load(FrameworkConfigManager*) : Build light array with size [" << arraySize << "].";
	
	lightArray.freq = TARGET_FREQ;
	lightArray.dmanum = DMA;
	lightArray.channel[0].gpionum = GPIO_PIN;
	lightArray.channel[0].count = arraySize;
	lightArray.channel[0].invert = 0;
	lightArray.channel[0].brightness = 0;
	lightArray.channel[0].strip_type = STRIP_TYPE;
	lightArray.channel[1].gpionum = 0;
	lightArray.channel[1].count = 0;
	lightArray.channel[1].invert = 0;
	lightArray.channel[1].brightness = 0;

	ws2811_return_t ret;
	if ((ret = ws2811_init(&lightArray)) != WS2811_SUCCESS)
	{
		LOG(LogLevel::Error) << "WS2812v10Renderer::load(FrameworkConfigManager*) : Initializing WS2812 failed with error no. [" << ws2811_get_return_t_str(ret) << "].";

		throw runtime_error("WS2812v10Renderer::load(FrameworkConfigManager*) : Initializing WS2812 failed.");

		fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
		//return ret;
		// TODO: 噴錯誤
	}

	return 0;
}

WS2812v10Renderer::WS2812v10Renderer(): Renderer(), RegisterType("WS2812v10Renderer")
{
	registerLoad(bind(static_cast<int(WS2812v10Renderer::*)(void)>(&WS2812v10Renderer::load), this));
}

int WS2812v10Renderer::Render()
{
	LOG(LogLevel::Finest) << "WS2812v10Renderer::Render() : Rendering ... ";
	// 陣列中的目前位置
	int tempPos = 0;

	uint8_t** matrix = lightMap->GetMatrix();

	LOG(LogLevel::Finest) << [](int width, int height, uint8_t** matrix){
		for (int i = 0; i < width; i++) {
			string s;
			for (int j = 0; j < height; j++) {
				s += to_string(matrix[i][j]);
				s += " ";
			}
			LOG(LogLevel::Finest) << "WS2812v10Renderer::Render() : light map | " << s << "|";
		}
		return 0;
	}(width, height, matrix);

	for (int i = 0; i < width; i++) {

		// 判斷白建和黑建，來設定鍵長
		bool whiteKey = isWhiteKey(startPitch + i);

		if (whiteKey) {

			for (int j = 0; j < height; j++) {
				lightArray.channel[0].leds[tempPos + j] = matrix[i][height - j - 1];
			}

			//memcpy(
			//	&lightArray.channel[0].leds[tempPos],
			//	&matrix[i][0],
			//	height);

			tempPos += height;
		}
		else {

			for (int j = 0; j < blackKeyHeight; j++) {
				lightArray.channel[0].leds[tempPos + j] = matrix[i][height - j - 1];
			}

			//memcpy(
			//	&lightArray.channel[0].leds[tempPos],
			//	&matrix[i][height - blackKeyHeight],
			//	blackKeyHeight);

			tempPos += blackKeyHeight;
		}
		
		//LOG(LogLevel::Finest) << [](int tempPos, bool isWhite, int height, int blackKeyHeight, ws2811_t* lightArray) {
		//	string s;
		//	for (int j = 0; j < isWhite ? height : blackKeyHeight; j++) {
		//		s += to_string(lightArray->channel[0].leds[tempPos+ j]);
		//		s += " ";
		//	}
		//	LOG(LogLevel::Finest) << "WS2812v10Renderer::Render() : light array | " << s << "|";
		//	return 0;
		//}(tempPos, whiteKey, height, blackKeyHeight, &lightArray);

	}

	return 0;
}

int WS2812v10Renderer::SendToDriver()
{
	LOG(LogLevel::Finest) << "WS2812v10Renderer::SendToDriver() : Sending out light array...";

	ws2811_return_t ret;
	if ((ret = ws2811_render(&lightArray)) != WS2811_SUCCESS)
	{
		LOG(LogLevel::Error) << "WS2812v10Renderer::SendToDriver() : rendering failed with error no. [" << ws2811_get_return_t_str(ret) << "].";

		throw runtime_error("WS2812v10Renderer::SendToDriver() : rendering failed.");

		fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
		// TODO: 噴錯誤
	}

	return 0;
}

bool WS2812v10Renderer::isWhiteKey(int key)
{
	switch (key % 12) {
	case 0:
	case 2:
	case 4:
	case 5:
	case 7:
	case 9:
	case 11:
		return true;
		break;
	default:
		return false;
		break;
	}

}
