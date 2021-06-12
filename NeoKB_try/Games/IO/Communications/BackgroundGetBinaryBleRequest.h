#ifndef BACKGROUND_GET_BINARY_BLE_REQUEST_H
#define BACKGROUND_GET_BINARY_BLE_REQUEST_H

#include "GetBinaryBleRequest.h"


using namespace Util::DataStructure;


namespace Games {
namespace IO{
namespace Communications{

	class BackgroundGetBinaryBleRequest : public GetBinaryBleRequest {

	public:

		BackgroundGetBinaryBleRequest(string fPath,
			MeteoBluetoothMessage* gMessage,
			MeteoCommand ackGetCommand,
			MeteoCommand tCommand,
			MeteoCommand aTransferCommand,
			MeteoCommand fCommand,
			MeteoCommand rRetransferCommand,
			MeteoCommand aFinishCommand);

		/// <summary>
		/// ���ɤ@��request�i�H�P�ɳQwifi�Bble�Bbt���h��communication component����A�o��request�n�������ثe�i��Ϊ�component�����ǡA�ۤv��w�n�έ���component
		/// �M��A����
		/// </summary>
		virtual int ChooseCommunicationComponentToPerform();

	protected:

		/// <summary>
		/// get binary request������ʧ@
		/// </summary>
		class BackgroundGetBinaryBleRequestMethod : public GetBinaryBleRequestMethod {

		public:

			/// <summary>
			/// ��Ƨ���m�̫᭱���n�[�W�׽u
			/// </summary>
			BackgroundGetBinaryBleRequestMethod(string fPath,
				MeteoBluetoothMessage* gMessage,
				MeteoCommand aGetCommand,
				MeteoCommand tCommand,
				MeteoCommand aTransferCommand,
				MeteoCommand fCommand,
				MeteoCommand rRetransferCommand,
				MeteoCommand aFinishCommand);

		};

	};

}}}

#endif