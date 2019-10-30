#ifndef METEO_DISPLAY_DEVICE_H
#define METEO_DISPLAY_DEVICE_H



#include "../../Framework/Devices/DisplayDevice.h"

using namespace Framework::Devices;



namespace Desktop {
namespace Devices {

	class MeteoDisplayDevice : public DisplayDevice {

	public:

		int Show(Map* m);

		/// <summary>
		/// �Ainput thread�C�@���ȧΤ@���A���Pdevice�ۤv�h�w�o�@���L�n������ѥX
		/// </summary>
		virtual int ProcessOutput();

	};

}}





#endif