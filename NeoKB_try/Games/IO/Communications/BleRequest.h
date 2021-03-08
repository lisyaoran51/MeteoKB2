#ifndef BLE_REQUEST_H
#define BLE_REQUEST_H

#include "../../../Framework/IO/Communications/CommunicationRequest.h"
#include <exception>
#include "../../Output/Bluetooths/MeteoBluetoothMessage.h"
#include "BleRequestException.h"
#include "../../../ThirdParty/json/json.hpp"


using namespace std;
using namespace Framework::IO::Communications;
using namespace Games::Output::Bluetooths;

using json = nlohmann::json;


namespace Games {
namespace IO{
namespace Communications{

	enum class BleRequestMethodType {
		None,
		PostText,
		PostBinary,
		GetText,
		GetBinary,
	};


	class BleRequest : virtual public CommunicationRequest {

	public:

		/// <summary>
		/// ���ɤ@��request�i�H�P�ɳQwifi�Bble�Bbt���h��communication component����A�o��request�n�������ثe�i��Ϊ�component�����ǡA�ۤv��w�n�έ���component
		/// �M��A����
		/// </summary>
		virtual int ChooseCommunicationComponentToPerform();

		virtual int Perform(CommunicationComponent* cComponent);

		/// <summary>
		/// ��s��raw message��i�h
		/// </summary>
		int PushInputRawMessage(MeteoBluetoothMessage* rawMessage);

	protected:


		BleRequestMethodType requestMethodType = BleRequestMethodType::None;

		deque<MeteoBluetoothMessage*> inputRawMessages;

		deque<MeteoBluetoothMessage*> inputRawMessagesBuffer;

		mutable mutex rawMessageMutex;

		mutable mutex rawMessageBufferMutex;

		/// <summary>
		/// �n���n�ۤv��on fail.trigger�\��o�̭��Aon fail�����\�bpublic Fail��
		/// </summary>
		virtual int fail(exception& e);


		class BleRequestMethod;

		BleRequestMethod* requestMethod = nullptr;


		/* ------------- BleRequestMethod ------------- */

		class BleRequestMethod {
		public:

			virtual int PerformAndWait(BleRequest* thisRequest) = 0;

			virtual BleRequestMethodType GetMethodType() = 0;

		};

	};

}}}

#endif