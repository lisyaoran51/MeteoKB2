#include "PostTextBleRequest.h"

#include "BleAccess.h"
#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Framework/Scenes/SceneMaster.h"

using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;
using namespace Framework::Scenes;


PostTextBleRequest::PostTextBleRequestMethod::PostTextBleRequestMethod(MeteoBluetoothMessage* pMessage) : postMessage(pMessage)
{
	isNeedCheckAck = false;
}

PostTextBleRequest::PostTextBleRequestMethod::PostTextBleRequestMethod(MeteoBluetoothMessage* pMessage, MeteoCommand aCommand) : postMessage(pMessage)
{
	ackCommand = aCommand;
}

int PostTextBleRequest::PostTextBleRequestMethod::PerformAndWait(BleRequest* thisRequest)
{
	PostTextBleRequest* thisPostTextRequest = dynamic_cast<PostTextBleRequest*>(thisRequest);

	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisPostTextRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<BleAccess*>(thisPostTextRequest->communicationComponent)->GetPeripheral());

	MeteoContextBluetoothMessage* outputMessage = nullptr;
	// TODO: command to message
	bluetoothPhone->PushOutputMessage(outputMessage);

	if (isNeedCheckAck) {

		while (1) {

			if (thisPostTextRequest->getElapsedSeconds() > thisPostTextRequest->timeout) {
				throw BleRequestException(BleResponseCode::RequestTimeout);
			}

			/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
			unique_lock<mutex> uLock(thisPostTextRequest->rawMessageMutex);
			for (int i = 0; i < thisPostTextRequest->inputRawMessages.size(); i++) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisPostTextRequest->inputRawMessages[i])) {
					if (dynamic_cast<MeteoContextBluetoothMessage*>(thisPostTextRequest->inputRawMessages[i])->GetCommand() == ackCommand) {

						/* 檢查Scene是否還存在，存在才能執行 */
						if (SceneMaster::GetInstance().CheckScene(thisPostTextRequest->callbackScene))
							onAck.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(thisPostTextRequest->inputRawMessages[i])->GetContextInJson());

						return 0;

					}
				}
				delete thisPostTextRequest->inputRawMessages[i];
			}
			thisPostTextRequest->inputRawMessages.clear();

			uLock.unlock();

			this_thread::sleep_for(std::chrono::milliseconds(100));

			/* 舊的程式，寫得不好
			mutex* inputRawCommandMutex = bleAccess->GetRawCommandMutex();
			unique_lock<mutex> uLock(*inputRawCommandMutex);

			for (int i = 0; i < bleAccess->GetInputRawCommand().size(); i++) {
				if (dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()[i])) {
					if (dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()[i])->GetCommand() == ackCommand) {

						onAck.TriggerThenClear(dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()[i])->GetContextInJson());

						bleAccess->GetInputRawCommand()->erase(bleAccess->GetInputRawCommand()->begin() + i);
						return 0;

					}
				}
			}
			uLock.unlock();
			*/
		}
	}


	return 0;
}

BleRequestMethodType PostTextBleRequest::PostTextBleRequestMethod::GetMethodType()
{
	return BleRequestMethodType::PostText;
}

int PostTextBleRequest::PostTextBleRequestMethod::AddOnAck(BleRequest* thisRequest, MtoObject * callableObject, function<int(json)> callback, string name)
{

	PostTextBleRequest* thisPostTextRequest = dynamic_cast<PostTextBleRequest*>(thisRequest);

	/* 不是scene就不能用request的callback */
	if (dynamic_cast<Scene*>(callableObject) == nullptr)
		return -1;

	/* 紀錄callback scene */
	if (thisPostTextRequest->callbackScene == nullptr)
		thisPostTextRequest->callbackScene = dynamic_cast<Scene*>(callableObject);

	/* 不允許由不同scene註冊，會當作error */
	if (thisPostTextRequest->callbackScene != dynamic_cast<Scene*>(callableObject))
		return -1;

	return onAck.Add(callableObject, callback, name);
}