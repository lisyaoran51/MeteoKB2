#ifndef PERIPHERAL_H
#define PERIPHERAL_H



#include <vector>
#include "../Input/InputState.h"

namespace Framework {
namespace Devices{
	class Device;
}}

using namespace std;
using namespace Framework::Devices;
using namespace Framework::Input;




namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class Peripheral {
		int aa;

	public:

		virtual int TriggerOnInput() = 0;

		virtual int SetDevice(Device* device);

		/// <summary>
		/// 給device用的，他拿到輸入以後就push給這邊，這邊才能再去叫input handler
		/// </summary>
		int PushInputState(InputState* inputState);

	protected:

		Device* matchedDevice;

		vector<InputState*> inputStates;

	private:




	};


}}









#endif