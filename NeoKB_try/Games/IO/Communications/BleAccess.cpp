#include "BleAccess.h"

#include <iterator>
//#include "IdentifyBleRequest.h"
//#include "FaultIdentityBleRequest.h"



using namespace Games::IO::Communications;


BleAccess::BleAccess(Host * gHost): TCommunicationComponent(gHost), RegisterType("BleAccess")
{
	bluetoothPhone = host->GetMainInterface()->GetBluetoothPhone();
	communicationState = CommunicationState::Failed;

	communicationThread = new GameThread(bind(&BleAccess::run, this), "BleRequestThread");
	communicationThread->SetMaxUpdateHz(100);

	communicationThread->Start();

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

BluetoothPhone * BleAccess::GetBluetoothPhone()
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

int BleAccess::HandleState(InputState * inputEvent)
{
	if (inputEvent->GetBluetoothState()->GetMessages()->size() > 0)
		handleOnRawMessage(inputEvent);

	return 0;
}

int BleAccess::run()
{
	threadExitRequest = false;

	while (!threadExitRequest) {
		switch (communicationState) {
		case CommunicationState::Failed:
			this_thread::sleep_for(std::chrono::milliseconds(500));
			
			// TODO: ping�@�U�ݬݦ��S���s�W
			continue;
			break;

		case CommunicationState::Offline:
		case CommunicationState::Connecting:

			communicationState = CommunicationState::Connecting;

			// ���n�J�ݬݡA�O���i�h�N�ରConnected�C�n�J��k�O��@��key�L�h�A������ӸѽX
			/*
			CommunicationRequest* request = new IdentifyBleRequest("aaaaaaaa");

			int identifyResult = handleRequest(request);

			delete request;
			request = nullptr;

			if (identifyResult == 0) {		// �ѽX���\

				communicationState = CommunicationState::Connected;

				break;
			}
			else if (identifyResult < 0) {	// timeout

				this_thread::sleep_for(std::chrono::milliseconds(500));

				continue;
			}
			else if (identifyResult == 1) {	// �ѽX����


				// �ѽX���~�A�_�u
				CommunicationRequest* disconnectRequest = new FaultIdentityBleRequest("Authorization failed.");
				handleRequest(disconnectRequest);

				delete disconnectRequest;
				disconnectRequest = nullptr;

				this_thread::sleep_for(std::chrono::milliseconds(500));
				// �n�Nble�s���_�}

				communicationState = CommunicationState::Failed;
				continue;
			}
			*/

		}


		/* �A����request */
		CommunicationRequest* request = nullptr;
		if (communicationRequests.size() > 0) {
			request = communicationRequests.back();

			int result = 0;

			result = handleRequest(request);		// ����handleRequest�Aresult�N�|�ܦ�undeclared

			if (result >= 0) {						// 0:���榨�\�A1:���楢��(�@�˭n��o��request�R��)
				communicationRequests.pop_back();

				delete request;
				request = nullptr;
			}
			else if (result == -1) {				// timeout�T���A�|����flush�Ҧ�request
				continue;
			}
		}
	}


	return 0;
}

int BleAccess::handleRequest(CommunicationRequest * communicationRequest)
{
	try {

		LOG(LogLevel::Fine) << "BleAccess::handleRequest() : run request [" << communicationRequest << "].";

		communicationRequest->Perform(this);

		failureCount = 0;

		return 0;

	}
	catch (BleRequestException& e) {

		BleResponseCode response = e.GetBleResponseCode();

		switch (response) {
		case BleResponseCode::RequestTimeout:
			failureCount++;

			if (failureCount < 3)
				return -1;

			// �p�G�@¾timeout�A�N��Ҧ���request��Fail��
			communicationState == CommunicationState::Failed;
			Flush();
			return -1;

			break;
		}

		communicationRequest->Fail(&e);

	}
	catch (exception& e) {

		LOG(LogLevel::Debug) << "BleAccess::handleRequest() : run request [" << communicationRequest << "] failed." << e.what();
		communicationRequest->Fail(&e);
	}

	return 1;	// �N����楢��
}

int BleAccess::handleOnRawMessage(InputState * inputState)
{
	/* ��raw command�ᵹ�Ҧ���request�A���L�̦ۤv�M�w�n���n�� */
	unique_lock<mutex> uLock(bleRequestMutex);
	for (int i = 0; i < bleRequests.size(); i++) {
		if (inputState->GetBluetoothState()->GetMessages()->size() > 0) {
			for (int j = 0; j < inputState->GetBluetoothState()->GetMessages()->size(); j++) {
				if (dynamic_cast<MeteoBluetoothMessage*>(inputState->GetBluetoothState()->GetMessages()->at(j)) == nullptr)
					continue;
				bleRequests[i]->PushInputRawMessage(dynamic_cast<MeteoBluetoothMessage*>(inputState->GetBluetoothState()->GetMessages()->at(j)->Clone()));
			}
		}
	}

	return 0;
}
