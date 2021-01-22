#include "BleAccess.h"

#include <iterator>


using namespace Games::IO::Communications;


BleAccess::BleAccess(GameHost * gHost): TCommunicationComponent(gHost), RegisterType("BleAccess")
{
	bluetoothPhone = host->GetMainInterface()->GetBluetoothPhone();


}

int BleAccess::Update()
{
	CommunicationComponent::Update();

	/* ��buffer�̭���raw command���M�X�� */
	/* defer_lock�N���l�Ʈɥ������o��lock */
	unique_lock<mutex> uLockBuffer(rawCommandBufferMutex, defer_lock);

	if (uLockBuffer.try_lock()) {
		if (inputRawCommandBuffer.size() > 0) {

			/* defer_lock�N���l�Ʈɥ������o��lock */
			unique_lock<mutex> uLock(rawCommandMutex, defer_lock);
			if (uLock.try_lock()) {

				/*
				 *	�o�䦳�Ӥ����n��bug�A�N�O�p�G��front_inserter�ӥ[�J�������ܡA�s�[�J���������Ƿ|��쥻�Adeque�̪����ǧ����ۤ�
				 *	���L�]��buffer�̭��������q�`�D�`��(���ӳq�`�u���@��)�A�ҥH���Φb�N�o��bug
				 */
				std::move(std::begin(inputRawCommandBuffer), std::end(inputRawCommandBuffer), std::front_inserter(inputRawCommand));
				inputRawCommandBuffer.clear();

				uLock.unlock();
			}
		}
		uLockBuffer.unlock();
	}

	return 0;
}

Peripheral * BleAccess::GetPeripheral()
{
	return bluetoothPhone;
}

int BleAccess::RegisterBleRequest(BleRequest * bleRequest)
{
	unique_lock<mutex> uLock(bleRequestMutex);

	bleRequests.push_back(bleRequest);

	return 0;
}

int BleAccess::UnregisterBleRequest(BleRequest * bleRequest)
{
	unique_lock<mutex> uLock(bleRequestMutex);

	for (int i = 0; i < bleRequests.size(); i++) {
		if (bleRequests[i] == bleRequest) {
			bleRequests.erase(bleRequests.begin() + i);
			i--;
		}
	}

	return 0;
}

deque<BluetoothMessage*>* BleAccess::GetInputRawCommand()
{
	// TODO: �󦹳B���J�Ǧ^���z��
	return &inputRawCommand;
}

mutex * BleAccess::GetRawCommandMutex()
{
	return &rawCommandMutex;
}

int BleAccess::GetMtu()
{
	// TODO: �T�wnotify���B�@�H��A�ӧ�o�q�ɧ�
	// ios �ª�mtu158�A�s��185�A��Ӿ��156(�������Y28�H���n�O128)
	return 156;
}

int BleAccess::run()
{
	//switch (communicationState) {
	//case CommunicationState::Offline:
	//	this_thread::sleep_for(std::chrono::milliseconds(500));
	//	return -1;
	//	break;
	//
	//case CommunicationState::Connecting:
	//
	//	// if login
	//	communicationState = CommunicationState::Connected;
	//	// else sleep(500); return -1;
	//
	//	break;
	//
	//}
	//
	//
	/* �A����request */
	CommunicationRequest* request = nullptr;
	if (communicationRequests.size() > 0) {
		request = communicationRequests.back();
	
		// request�B�z���\
		if (handleRequest(request) >= 0) {
			communicationRequests.pop_back();
		}
		//else 
		//	return -1;
		
	}

	return 0;
}

int BleAccess::handleRequest(CommunicationRequest * communicationRequest)
{
	try {

		LOG(LogLevel::Fine) << "BleAccess::handleRequest() : run request [" << communicationRequest << "].";

		communicationRequest->Perform(this);

		failureCount = 0;

	}
	catch (BleRequestException& e) {

		BleResponseCode response = e.GetBleResponseCode();

		switch (response) {
		case BleResponseCode::RequestTimeout:
			failureCount++;

			if (failureCount < 3)
				return -1;

			communicationState == CommunicationState::Failed;
			Flush();
			return 0;

			break;
		}

		communicationRequest->Fail(e);

	}

	return 0;
}

int BleAccess::handleOnRawCommand(InputState * inputState)
{
	/* ��raw command�ᵹ�Ҧ���request�A���L�̦ۤv�M�w�n���n�� */
	unique_lock<mutex> uLock(bleRequestMutex);
	for (int i = 0; i < bleRequests.size(); i++) {
		if (inputState->GetBluetoothState()->GetMessages()->size() > 0) {
			for (int j = 0; j < inputState->GetBluetoothState()->GetMessages()->size(); j++) {
				bleRequests[i]->PushInputRawMessage(inputState->GetBluetoothState()->GetMessages()->at(j)->Clone());
			}
		}
	}

	return 0;
	// --------------------�᭱�O�ª������A�g�o���n

	/* defer_lock�N���l�Ʈɥ������o��lock */
	unique_lock<mutex> uLock(rawCommandMutex, defer_lock);


	if (uLock.try_lock()) {

		if (inputState->GetBluetoothState()->GetMessages()->size() > 0) {
			for (int i = 0; i < inputState->GetBluetoothState()->GetMessages()->size(); i++) {
				inputRawCommand.push_front(inputState->GetBluetoothState()->GetMessages()->at(i)->Clone());
			}
		}

		uLock.unlock();
	}
	else {
		/* inputRawCommand���b�Q�ϥΤ����ܡA�N����s��command���buffer�� */
		unique_lock<mutex> uLockBuffer(rawCommandBufferMutex);

		if (inputState->GetBluetoothState()->GetMessages()->size() > 0) {
			for (int i = 0; i < inputState->GetBluetoothState()->GetMessages()->size(); i++) {
				inputRawCommandBuffer.push_front(inputState->GetBluetoothState()->GetMessages()->at(i)->Clone());
			}
		}
		uLockBuffer.unlock();


	}

	

	return 0;
}
