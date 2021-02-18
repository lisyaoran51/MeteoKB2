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