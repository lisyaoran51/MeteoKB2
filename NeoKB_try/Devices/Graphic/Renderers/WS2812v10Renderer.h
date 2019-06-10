#ifndef WS2812_VER_10_RENDERER_H
#define WS2812_VER_10_RENDERER_H


#include "../../../Base/Graphic/Renderer/renderer.h"
#include "../../LedDriver/ws2811.h"


using namespace Base::Graphic::Renderers;


namespace Devices {
namespace Graphics {
namespace Renderers {

	class WS2812v10Renderer: public Renderer {
	
		int load();

		int load(FrameworkConfigManager* f);

	public:

		WS2812v10Renderer();

		virtual int Render();

		virtual int SendToDriver();

	protected:

		int startPitch;

		int width;

		int height;

		int blackKeyHeight;

		ws2811_t lightArray;

		bool isWhiteKey(int key);

	};

}}}








#endif