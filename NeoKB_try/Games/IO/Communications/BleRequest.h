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
		/// 有時一個request可以同時被wifi、ble、bt等多種communication component執行，這時request要先接收目前可選用的component有哪些，自己選定要用哪種component
		/// 然後再執行
		/// </summary>
		virtual int ChooseCommunicationComponentToPerform();

		virtual int Perform(CommunicationComponent* cComponent);

		/// <summary>
		/// 把新的raw message丟進去
		/// </summary>
		int PushInputRawMessage(MeteoBluetoothMessage* rawMessage);

	protected:


		BleRequestMethodType requestMethodType = BleRequestMethodType::None;

		deque<MeteoBluetoothMessage*> inputRawMessages;

		deque<MeteoBluetoothMessage*> inputRawMessagesBuffer;

		mutable mutex rawMessageMutex;

		mutable mutex rawMessageBufferMutex;

		/// <summary>
		/// 好像要自己把on fail.trigger擺到這裡面，on fail不能擺在public Fail裡
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