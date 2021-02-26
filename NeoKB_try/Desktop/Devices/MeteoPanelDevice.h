#ifndef METEO_PANEL_DEVICE_H
#define METEO_PANEL_DEVICE_H


#include "../../Framework/Devices/PanelDevice.h"
#include "MeteoMcuV1.h"



using namespace Framework::Devices;



namespace Desktop {
namespace Devices {

	class MeteoPanelDevice : public PanelDevice {

		MeteoMcuV1 * meteoMcu = nullptr;

		int _debugCount = 0;

	public:

		MeteoPanelDevice(MeteoMcuV1* mMcu);


	protected:

		virtual int readFromDevice();

		virtual int passToDevice();

	};

}}





#endif