#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include "Peripheral.h"
#include "../Graphic/Map/Map.h"

namespace Framework {
namespace Devices {
	class Device;
	class DisplayDevice;
}}

using namespace std;
using namespace Framework::Graphic::Maps;
using namespace Framework::Devices;



namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class Display : public Peripheral {

		

	public:

		Display();

		virtual int SetDevice(Device* device);
		
		virtual int TriggerOnInput();

		int Show(Map* m);

	protected:

		DisplayDevice* matchedDisplayDevice;

	private:




	};


}}


#endif