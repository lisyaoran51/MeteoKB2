#ifndef PERIPHERAL_H
#define PERIPHERAL_H



#include <vector>
#include "../Devices/Device.h"

using namespace std;
using namespace Framework::Devices;

namespace Framework {
namespace IO {

	/// <summary>
	/// �����J���a��A�ѥX���a��
	/// </summary>
	class Peripheral {


	public:

		virtual int TriggerOnInput() = 0;

		virtual int SetDevice(Device* device);

		/// <summary>
		/// ��device�Ϊ��A�L�����J�H��Npush���o��A�o��~��A�h�sinput handler
		/// </summary>
		int PushInputState(InputState* inputState);

	protected:

		Device* matchedDevice;

		vector<InputState*> inputStates;

	private:




	};


}}









#endif