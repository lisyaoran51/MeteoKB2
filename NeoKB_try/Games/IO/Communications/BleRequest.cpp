#include "BleRequest.h"

#include "BleAccess.h"
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



int BleRequest::Perform(CommunicationComponent * cComponent)
{
	communicationComponent = cComponent;

	BleAccess* bleAccess = dynamic_cast<BleAccess*>(communicationComponent);
	// 讓ble access把raw message丟進來，好讓我們檢查有沒有ack或return
	bleAccess->RegisterBleRequest(this);

	requestStartTime = system_clock::now();

	// preform 丟資訊出去然後等回覆

	requestMethod->PerformAndWait(this);

	// 執行完畢以後就不讓ble access把raw message丟進來
	bleAccess->UnregisterBleRequest(this);

	// 如果有出現錯誤，會丟exception，就不會執行on success。這邊要注意request在執行完以後不能直接刪掉，要確定所有task都跑完才能刪
	communicationComponent->GetScheduler()->AddTask([=]() {
		/* 檢查Scene是否還存在，存在才能執行 */
		if(SceneMaster::GetInstance().CheckScene(callbackScene))
			onSuccess.TriggerThenClear();
		return 0;
	});

	return 0;
}

int BleRequest::PushInputRawMessage(MeteoBluetoothMessage * rawMessage)
{
	bool isAcceptMessage = false;

	/* 如果是file segment的話，只有get file request要收。其他狀況就全都收 */
	if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(rawMessage)) {
		if (requestMethod->GetMethodType() == BleRequestMethodType::GetBinary) {
			isAcceptMessage = true;
		}
	}
	else {
		isAcceptMessage = true;
	}

	if (isAcceptMessage) {

		unique_lock<mutex> uLock(rawMessageMutex, defer_lock);
		if (uLock.try_lock()) {
			inputRawMessages.push_front(rawMessage);
			uLock.unlock();
		}
		else {
			unique_lock<mutex> uBufferLock(rawMessageBufferMutex);
			inputRawMessagesBuffer.push_front(rawMessage);
		}
	}
	else {
		/* 因為這邊的raw message全都是複製過來的，所以不用的話要delete調 */
		delete rawMessage;
		rawMessage = nullptr;
	}

	return 0;
}



