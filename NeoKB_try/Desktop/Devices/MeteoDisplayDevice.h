#ifndef METEO_DISPLAY_DEVICE_H
#define METEO_DISPLAY_DEVICE_H



#include "../../Framework/Devices/DisplayDevice.h"
#include "MeteoLightBoardV1.h"


using namespace Framework::Devices;


namespace Desktop {
namespace Devices {

	class MeteoDisplayDevice : public DisplayDevice {

		int width;
		int height;

		MeteoLightBoardV1* meteoLightBoard;

		uint8_t** lightMatrix;

	public:

		MeteoDisplayDevice(int w, int h);



		int Show(Map* m);

		/// <summary>
		/// 再input thread每一輪值形一次，不同device自己去定這一輪他要做什麼書出
		/// </summary>
		virtual int ProcessOutput();

	};

}}





#endif