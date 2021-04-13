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


		/// <summary>
		/// �Nbuffer����raw command��s�icommand��
		/// </summary>
		virtual int Update();

		Peripheral* GetPeripheral();

		BluetoothPhone* GetBluetoothPhone();

		int RegisterBleRequest(BleRequest* bleRequest);

		int UnregisterBleRequest(BleRequest* bleRequest);

		/// <summary>
		/// �R���ɭn���O��thread safe?
		/// �νƻs���O�̦w������k�A���O�Ĳv�����D�|�C�h��
		/// �ƻs���٭nclear
		/// </summary>
		deque<BluetoothMessage*>* GetInputRawCommand();

		mutex* GetRawCommandMutex();

		/// <summary>
		/// ����ثe�]�w��mtu�A�o���٭n�T�{norify�O���B�@�~���D�ण��o�˼g
		/// </summary>
		int GetMtu();

		/* --------------CommunicationComponent.h-------------- */

		virtual int HandleState(InputState* inputEvent);

	protected:

		BluetoothPhone* bluetoothPhone = nullptr;

		vector<BleRequest*> bleRequests;

		mutable mutex bleRequestMutex;

		deque<BluetoothMessage*> inputRawCommand;

		/// <summary>
		/// ���ɭ�handle on new command�ɭ��ninput raw command�Q�O��request��lock�_�ӡA�N�n����input command����i�o��buffer�̭��A
		/// ����A��ɶ���buffer���e��^inputRawCommand
		/// </summary>
		deque<BluetoothMessage*> inputRawCommandBuffer;

		mutable mutex rawCommandMutex;

		mutable mutex rawCommandBufferMutex;

		virtual int run();

		/// <summary>
		/// �o��n��strategy�B�zwifi�Mble�P�ɳB�z�����D
		/// </summary>
		virtual int handleRequest(CommunicationRequest* communicationRequest);

		/// <summary>
		/// ��bluetooth phone�s���쪺�Ҧ��T����clone�@���i��
		/// �W�L�@�w�ɶ��S��input���ɭԴN�|�M�šA�t�~raw command�]�|�O���@�w�ƶq�A�W�X�ƶq�N�|�R��
		/// �R�����ɭԭn���O��thread safe?
		/// </summary>
		int handleOnRawMessage(InputState* inputState);
		

	};

}}}



#endif