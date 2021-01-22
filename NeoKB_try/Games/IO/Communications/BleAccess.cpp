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
	/* 再執行request */
	CommunicationRequest* request = nullptr;
	if (communicationRequests.size() > 0) {
		request = communicationRequests.back();
	
		// request處理成功
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
	/* 把raw command丟給所有的request，讓他們自己決定要不要接 */
	unique_lock<mutex> uLock(bleRequestMutex);
	for (int i = 0; i < bleRequests.size(); i++) {
		if (inputState->GetBluetoothState()->GetMessages()->size() > 0) {
			for (int j = 0; j < inputState->GetBluetoothState()->GetMessages()->size(); j++) {
				bleRequests[i]->PushInputRawMessage(inputState->GetBluetoothState()->GetMessages()->at(j)->Clone());
			}
		}
	}

	return 0;
	// --------------------後面是舊的城市，寫得不好

	/* defer_lock代表初始化時先不鎖住這個lock */
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
		/* inputRawCommand正在被使用中的話，就先把新的command丟到buffer中 */
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
