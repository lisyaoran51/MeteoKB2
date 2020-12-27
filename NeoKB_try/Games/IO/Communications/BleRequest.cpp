#include "BleRequest.h"


using namespace Games::IO::Communications;


BleResponseCode BleRequestException::GetBleResponseCode()
{
	return BleResponseCode();
}

int BleRequest::Perform(CommunicationComponent * cComponent)
{
	communicationComponent = cComponent;

	systemStartTime = system_clock::now();

	// preform 丟資訊出去然後等回覆

	requestMethod->PerformAndWait();

	// 如果有出現錯誤，會丟exception，就不會執行on success

	communicationComponent->GetScheduler()->AddTask([=]() {
		onSuccess.TriggerThenClear();
		return 0;
	});

	return 0;
}
