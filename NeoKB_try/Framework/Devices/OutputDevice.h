#ifndef OUTPUT_DEVICE_H
#define OUTPUT_DEVICE_H

#include "Device.h"



namespace Framework {
namespace Devices {

	class OutputDevice : virtual public Device {

	public:

		/// <summary>
		/// 再input thread每一輪值形一次，不同device自己去定這一輪他要做什麼書出
		/// </summary>
		virtual int ProcessOutput() = 0;

	protected:

		

	};

}}




#endif