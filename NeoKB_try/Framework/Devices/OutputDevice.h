#ifndef OUTPUT_DEVICE_H
#define OUTPUT_DEVICE_H

#include "Device.h"



namespace Framework {
namespace Devices {

	class OutputDevice : virtual public Device {

	public:

		/// <summary>
		/// �Ainput thread�C�@���ȧΤ@���A���Pdevice�ۤv�h�w�o�@���L�n������ѥX
		/// </summary>
		virtual int ProcessOutput() = 0;

	protected:

		

	};

}}




#endif