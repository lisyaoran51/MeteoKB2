#ifndef METEO_PANEL_DEVICE_H
#define METEO_PANEL_DEVICE_H


#include "../../Framework/Devices/PanelDevice.h"
#include "MeteoPanelBoardV1.h"



using namespace Framework::Devices;



namespace Desktop {
namespace Devices {

	class MeteoPanelDevice : public PanelDevice {

		MeteoPanelBoardV1 * meteoPanelBoard = nullptr;

		int _debugCount = 0;

	public:

		MeteoPanelDevice(MeteoPanelBoardV1* panelBoard);

		/// <summary>
		/// 再input thread每一輪值形一次，不同device自己去定這一輪他要做什麼書出
		/// </summary>
		virtual int ProcessOutput();

	protected:

		virtual int readFromDevice();

	};

}}





#endif