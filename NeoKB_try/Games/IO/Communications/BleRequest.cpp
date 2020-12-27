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

	// preform ���T�X�h�M�ᵥ�^��

	requestMethod->PerformAndWait();

	// �p�G���X�{���~�A�|��exception�A�N���|����on success

	communicationComponent->GetScheduler()->AddTask([=]() {
		onSuccess.TriggerThenClear();
		return 0;
	});

	return 0;
}
