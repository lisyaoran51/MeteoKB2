#ifndef BLE_ACCESS_H
#define BLE_ACCESS_H


#include "../../../Framework/IO/Communications/CommunicationComponent.h"
#include "BleRequest.h"
#include "../../../Framework/IO/BluetoothPhone.h"


using namespace Framework::IO::Communications;
using namespace Framework::IO;


namespace Games {
namespace IO{
namespace Communications{

	/// <summary>
	/// �o�Ӥ��\�bframework�A�O�]���L�|���@�Ǹ�app�j������T�A�Ҧpuser�Btoken�B�[�ѱK����
	/// �������~�A�p�G�i�H��o�Ǹ�app�j������T���j�}�A�o��class�N�i�H�ʦAframework�A���ݭn���ܦA��refactor
	/// </summary>
	class BleAccess : public TCommunicationComponent<BleRequest> {

	public:

		BleAccess(GameHost* gHost);

		Peripheral* GetPeripheral();

		deque<BluetoothCommand*>& GetInputRawCommand();

		/// <summary>
		/// ����ثe�]�w��mtu�A�o���٭n�T�{norify�O���B�@�~���D�ण��o�˼g
		/// </summary>
		int GetMtu();

	protected:

		BluetoothPhone* bluetoothPhone = nullptr;

		deque<BluetoothCommand*> inputRawCommand;

		virtual int run();

		/// <summary>
		/// �o��n��strategy�B�zwifi�Mble�P�ɳB�z�����D
		/// </summary>
		virtual int handleRequest(CommunicationRequest* communicationRequest);

		/// <summary>
		/// ��bluetooth phone�s���쪺�Ҧ��T����clone�@���i��
		/// </summary>
		int handleOnRawCommand(InputState* inputState);
		

	};

}}}



#endif