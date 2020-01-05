#ifndef METEO_BITBANG_DISPLAY_DEVICE_H
#define METEO_BITBANG_DISPLAY_DEVICE_H


#include "../../Framework/Devices/DisplayDevice.h"


using namespace Framework::Devices;


namespace Desktop {
namespace Devices{

	class MeteoBitbangDisplayDevice : public DisplayDevice {

		int width;
		int height;

		int lightBoardFileDescriptor = -1;

		char lightMatrixMessage[96];

		uint8_t** lightMatrix;

	public:

		MeteoBitbangDisplayDevice(int w, int h);

		int Show(Map* m);

		/// <summary>
		/// µL°Ê§@
		/// </summary>
		virtual int passToDevice();

	};

}}





#endif