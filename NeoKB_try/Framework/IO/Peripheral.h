#ifndef PERIPHERAL_H
#define PERIPHERAL_H



#include <vector>
#include "../Input/InputState.h"
#include "../Output//OutputMessage.h"
#include <mutex>


namespace Framework {
namespace Devices{
	class Device;
}}


using namespace std;
using namespace Framework::Devices;
using namespace Framework::Input;
using namespace Framework::Output;




namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class Peripheral {

	public:

		virtual int TriggerOnInput() = 0;

		virtual int SetDevice(Device* device);

		/// <summary>
		/// 給device用的，他拿到輸入以後就push給這邊，這邊才能再去叫input handler
		/// </summary>
		int PushInputState(InputState* inputState);

		/// <summary>
		/// 拿去註冊在io communicator裡，或是其他要接收output的地方
		/// </summary>
		int PushOutputMessage(OutputMessage* outputMessage);

		/// <summary>
		/// 把output全部倒進device裡
		/// </summary>
		int PourOutOutputMessages(vector<OutputMessage*>* pourOutTo);

	protected:

		mutable mutex outputMessageMutex;

		Device* matchedDevice;

		vector<InputState*> inputStates;

		vector<OutputMessage*> outputMessages;

	private:




	};


}}









#endif