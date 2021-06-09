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
	// TODO: 在連接時更改連線狀態

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

	/* 把buffer裡面的raw command先清出來 */
	/* defer_lock代表初始化時先不鎖住這個lock */
	unique_lock<mutex> uLockBuffer(rawCommandBufferMutex, defer_lock);

	if (uLockBuffer.try_lock()) {
		if (inputRawCommandBuffer.size() > 0) {

			/* defer_lock代表初始化時先不鎖住這個lock */
			unique_lock<mutex> uLock(rawCommandMutex, defer_lock);
			if (uLock.try_lock()) {

				/*
				 *	這邊有個不重要的bug，就是如果用front_inserter來加入元素的話，新加入的元素順序會跟原本再deque裡的順序完全相反
				 *	不過因為buffer裡面的元素通常非常少(應該通常只有一個)，所以不用在意這個bug
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
	// TODO: 於此處插入傳回陳述式
	return &inputRawCommand;
}

mutex * BleAccess::GetRawCommandMutex()
{
	return &rawCommandMutex;
}

int BleAccess::GetMtu()
{
	// TODO: 確定notify怎麼運作以後再來把這段補完
	// ios 舊版mtu158，新版185，抓個整數156(扣掉標頭28以後剛好是128)
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
			
			// TODO: ping一下看看有沒有連上
			continue;
			break;

		case CommunicationState::Offline:
		case CommunicationState::Connecting:
			LOG(LogLevel::Finest) << "BleAccess::run() : state offline.";

			communicationState = CommunicationState::Connecting;

			// 先登入看看，燈的進去就轉為Connected。登入方法是丟一個key過去，讓手機來解碼
			/*
			CommunicationRequest* request = new IdentifyBleRequest("aaaaaaaa");

			int identifyResult = handleRequest(request);

			delete request;
			request = nullptr;

			if (identifyResult == 0) {		// 解碼成功

				communicationState = CommunicationState::Connected;

				break;
			}
			else if (identifyResult < 0) {	// timeout

				this_thread::sleep_for(std::chrono::milliseconds(500));

				continue;
			}
			else if (identifyResult == 1) {	// 解碼失敗


				// 解碼錯誤，斷線
				CommunicationRequest* disconnectRequest = new FaultIdentityBleRequest("Authorization failed.");
				handleRequest(disconnectRequest);

				delete disconnectRequest;
				disconnectRequest = nullptr;

				this_thread::sleep_for(std::chrono::milliseconds(500));
				// 要將ble連接斷開

				communicationState = CommunicationState::Failed;
				continue;
			}
			*/

		}

		LOG(LogLevel::Finest) << "BleAccess::run() : handling reuqest.";

		if(communicationRequests.size() == 0)
			this_thread::sleep_for(std::chrono::milliseconds(100));

		/* 再執行request */
		CommunicationRequest* request = nullptr;
		if (communicationRequests.size() > 0) {
			LOG(LogLevel::Finest) << "BleAccess::run() : handling reuqest.";
			request = communicationRequests.back();

			int result = 0;

			result = handleRequest(request);		// 執行handleRequest，result就會變成undeclared

			if (result >= 0) {						// 0:執行成功，1:執行失敗(一樣要把這個request刪掉)
				communicationRequests.pop_back();

				delete request;
				request = nullptr;
			}
			else if (result == -1) {				// timeout三次，會直接flush所有request
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

			// 如果一職timeout，就把所有的request都Fail調
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

	return 1;	// 代表執行失敗
}

int BleAccess::handleOnRawMessage(InputState * inputState)
{
	/* 把raw command丟給所有的request，讓他們自己決定要不要接 */
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
