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

		/// <summary>
		/// ���h���U�bio communicator�̡A�άO��L�n����output���a��
		/// </summary>
		int PushOutputMessage(OutputMessage* outputMessage);

		/// <summary>
		/// ��output�����˶idevice��
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