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

	retryCount = 0;

	// preform ���T�X�h�M�ᵥ�^��

	return 0;
}
