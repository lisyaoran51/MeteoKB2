#include "BleAccess.h"

#include <iterator>
#include <string.h>
#include "../../../Framework/Threading/ThreadMaster.h"


using namespace Games::IO::Communications;
using namespace Framework::Threading;


BleAccess::BleAccess(Host * gHost): TCommunicationComponent(gHost), RegisterType("BleAccess")
{
	bluetoothPhone = host->GetMainInterface()->GetBluetoothPhone();
	communicationState = CommunicationState::Connected;
	// TODO: �b�s���ɧ��s�u���A

	thisThread = new thread(&BleAccess::run, this);
	sleepTimeInMilliSecond = 20;
	ThreadMaster::GetInstance().AddSimpleThread(this);

	int policy = SCHED_OTHER;
	struct sched_param param;
	memset(&param, 0, sizeof(param));
	param.sched_priority = sched_get_priority_min(policy);
	pthread_setschedparam(runThread->native_handle(), policy, &param);

	runThread->detach();

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

			LOG(LogLevel::Finest) << "BleAccess::run() : state failed.";
			this_thread::sleep_for(std::chrono::milliseconds(500));
			
			// TODO: ping�@�U�ݬݦ��S���s�W
			continue;
			break;

		case CommunicationState::Offline:
		case CommunicationState::Connecting:
			LOG(LogLevel::Finest) << "BleAccess::run() : state offline.";

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

		LOG(LogLevel::Finest) << "BleAccess::run() : handling reuqest.";

		if(communicationRequests.size() == 0)
			this_thread::sleep_for(std::chrono::milliseconds(100));

		/* �A����request */
		CommunicationRequest* request = nullptr;
		if (communicationRequests.size() > 0) {
			LOG(LogLevel::Finest) << "BleAccess::run() : handling reuqest.";
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

		LOG(LogLevel::Finest) << "BleAccess::handleRequest() : run request [" << communicationRequest << "].";

		communicationRequest->Perform(this);

		failureCount = 0;


		LOG(LogLevel::Finest) << "BleAccess::handleRequest() : trigger on success.";
		communicationRequest->Success();


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
