#ifndef METEO_PANEL_DEVICE_H
#define METEO_PANEL_DEVICE_H

#include "../../Framework/Devices/PanelDevice.h"

using namespace Framework::Devices;



namespace Desktop {
namespace Devices {

	class MeteoPanelDevice : public PanelDevice {

	public:

		/// <summary>
		/// �Ainput thread�C�@���ȧΤ@���A���Pdevice�ۤv�h�w�o�@���L�n������ѥX
		/// </summary>
		virtual int ProcessOutput();

	protected:

		virtual int readFromDevice();

	};

}}





#endif