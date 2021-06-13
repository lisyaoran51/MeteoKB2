#ifndef FOREGROUND_BLE_ACCESS_H
#define FOREGROUND_BLE_ACCESS_H


#include "TBleAccess.h"




namespace Games {
namespace IO{
namespace Communications{

	/// <summary>
	/// �o�Ӥ��\�bframework�A�O�]���L�|���@�Ǹ�app�j������T�A�Ҧpuser�Btoken�B�[�ѱK����
	/// �������~�A�p�G�i�H��o�Ǹ�app�j������T���j�}�A�o��class�N�i�H�ʦAframework�A���ݭn���ܦA��refactor
	/// </summary>
	class ForegroundBleAccess : public TBleAccess<BleRequest> {

	public:

		ForegroundBleAccess(Host* gHost);

		virtual int Queue(CommunicationRequest* communicationRequest);

		virtual int Flush();

	protected:

	};

}}}



#endif