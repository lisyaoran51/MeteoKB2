#include "BleRequest.h"

#include "ForegroundBleAccess.h"
#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Output/Bluetooths/MeteoFileSegmentBluetoothMessage.h"
#include <exception>
#include <fstream>
#include "../../../Util/DataStructure/FileSegmentMap.h"
#include "../../../Util/StringSplitter.h"
#include "../../../Framework/Scenes/SceneMaster.h"


using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;
using namespace std;
using namespace Util::DataStructure;
using namespace Util;
using namespace Framework::Scenes;



BleRequest::BleRequest() : RegisterType("BleRequest")
{
}

BleRequest::~BleRequest()
{
	for (int i = 0; i < inputRawMessages.size(); i++) {
		delete inputRawMessages[i];
	}
	inputRawMessages.clear();

	for (int i = 0; i < inputRawMessagesBuffer.size(); i++) {
		delete inputRawMessagesBuffer[i];
	}
	inputRawMessagesBuffer.clear();
}

int BleRequest::ChooseCommunicationComponentToPerform()
{

	LOG(LogLevel::Depricated) << "int BleRequest::ChooseCommunicationComponentToPerform() : [" << GetTypeName() << "] request finding component.";

	map<string, deque<CommunicationRequest*>*>::iterator it;

	for (it = acceptedCommunicationComponentRequestQueues.begin(); it != acceptedCommunicationComponentRequestQueues.end(); ++it) {
		if (it->first == "ForegroundBleAccess") {

			LOG(LogLevel::Depricated) << "int BleRequest::ChooseCommunicationComponentToPerform() : [" << GetTypeName() << "] component found.";
			it->second->push_back(this);
		}
	}


	return 0;
}

int BleRequest::Perform(CommunicationComponent * cComponent)
{

	LOG(LogLevel::Depricated) << "int BleRequest::Perform() : [" << GetTypeName() << "] start perform.";

	communicationComponent = cComponent;

	ForegroundBleAccess* bleAccess = dynamic_cast<ForegroundBleAccess*>(communicationComponent);
	// 讓ble access把raw message丟進來，好讓我們檢查有沒有ack或return
	bleAccess->RegisterBleRequest(this);

	requestStartTime = system_clock::now();
	exitRequested = false;

	// preform 丟資訊出去然後等回覆

	try {
		requestMethod->PerformAndWait(this);
	}
	catch (exception& e) {
		// 執行完畢以後就不讓ble access把raw message丟進來
		bleAccess->UnregisterBleRequest(this);
		throw e;
	}

	// 執行完畢以後就不讓ble access把raw message丟進來
	bleAccess->UnregisterBleRequest(this);

	// 查看成果有沒有錯，有錯的話就throw exception
	checkAndProcessFailure();

	return 0;
}

int BleRequest::PushInputRawMessage(MeteoBluetoothMessage * rawMessage)
{
	bool isAcceptMessage = true;
	LOG(LogLevel::Depricated) << "BleRequest::PushInputRawMessage() : new raw message." << hex << (int)rawMessage->GetCommand();

	/* 如果是file segment的話，只有get file request要收。其他狀況就全都收 */
	if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(rawMessage)) {

		LOG(LogLevel::Depricated) << "BleRequest::PushInputRawMessage() : new file segment message." << hex << (int)rawMessage->GetCommand();
		if (requestMethod->GetMethodType() == BleRequestMethodType::GetBinary) {
			isAcceptMessage = true;
		}
	}
	else {
		isAcceptMessage = true;
	}

	if (isAcceptMessage) {

		//unique_lock<mutex> uLock(rawMessageMutex, defer_lock);
		unique_lock<mutex> uLock(rawMessageMutex);
		inputRawMessages.push_front(rawMessage);
		//if (uLock.try_lock()) {
		//	inputRawMessages.push_front(rawMessage);
		//	uLock.unlock();
		//}
		//else {
		//	unique_lock<mutex> uBufferLock(rawMessageBufferMutex);
		//	inputRawMessagesBuffer.push_front(rawMessage);
		//}
	}
	else {
		/* 因為這邊的raw message全都是複製過來的，所以不用的話要delete調 */
		delete rawMessage;
		rawMessage = nullptr;
	}

	return 0;
}

int BleRequest::checkAndProcessFailure()
{
	return 0;
}

int BleRequest::fail(exception * e)
{

	if (dynamic_cast<CommunicationRequestException*>(e)) {

	}

	LOG(LogLevel::Debug) << "int BleRequest::fail() : not implemented.";
	return 0;
}
