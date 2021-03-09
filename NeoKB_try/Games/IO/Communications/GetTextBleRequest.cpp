#include "GetTextBleRequest.h"

#include "BleAccess.h"
#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Framework/Scenes/SceneMaster.h"


using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;
using namespace Framework::Scenes;



GetTextBleRequest::GetTextBleRequestMethod::GetTextBleRequestMethod(MeteoBluetoothMessage * gMessage, MeteoCommand rCommand) : getMessage(gMessage)
{
	returnCommand = rCommand;
}

int GetTextBleRequest::GetTextBleRequestMethod::PerformAndWait(BleRequest * thisRequest)
{

	GetTextBleRequest* thisGetTextRequest = dynamic_cast<GetTextBleRequest*>(thisRequest);

	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisGetTextRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<BleAccess*>(thisGetTextRequest->communicationComponent)->GetPeripheral());

	MeteoContextBluetoothMessage* outputMessage = nullptr;

	bluetoothPhone->PushOutputMessage(getMessage);

	while (1) {

		if (thisGetTextRequest->getElapsedSeconds() > thisGetTextRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisGetTextRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* 確認收到的command是不是return command */
		unique_lock<mutex> uLock(thisGetTextRequest->rawMessageMutex);
		while (thisGetTextRequest->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisGetTextRequest->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == returnCommand) {

					// 確認Scene還存不存在，如果不存在就不執行callback，避免該scene已經被刪掉的情況
					if ((thisGetTextRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetTextRequest->callbackScene)) ||
						!thisGetTextRequest->isCallbackByScene ||
						onReturn.GetSize() == 0)
						onReturn.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson());
					else
						throw BleRequestException(BleResponseCode::Gone);

					return 0;

				}
			}
			thisGetTextRequest->inputRawMessages.pop_back();
			delete message;
		}
		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));



		/* 這段成是寫得不好，不用了
		unique_lock<mutex> uLock(*bleAccess->GetRawCommandMutex());

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




	return 0;
}

BleRequestMethodType GetTextBleRequest::GetTextBleRequestMethod::GetMethodType()
{
	return BleRequestMethodType::GetText;
}

string GetTextBleRequest::GetTextBleRequestMethod::GetReturnText()
{
	return returnText;
}

json GetTextBleRequest::GetTextBleRequestMethod::GetReturnJson()
{
	return json(returnText);
}

int GetTextBleRequest::GetTextBleRequestMethod::AddOnReturn(MtoObject * callableObject, function<int(json)> callback, string name)
{

	onReturn.Add(callableObject, callback, name);
	return 0;
}

GetTextBleRequest::GetTextBleRequest() : RegisterType("GetTextBleRequest")
{
	// 用來override的
}

GetTextBleRequest::GetTextBleRequest(MeteoBluetoothMessage * gMessage, MeteoCommand rCommand) : RegisterType("GetTextBleRequest")
{
	GetTextBleRequestMethod* method = new GetTextBleRequestMethod(gMessage, rCommand);
	requestMethod = method;
}

int GetTextBleRequest::AddOnReturn(MtoObject * callableObject, function<int(json)> callback, string name)
{
	/* 檢查是不是由scene去add的 */
	if (dynamic_cast<Scene*>(callableObject)) {
		isCallbackByScene &= (dynamic_cast<Scene*>(callableObject) != nullptr);
		if (isCallbackByScene)
			callbackScene = dynamic_cast<Scene*>(callableObject);
		else
			callbackScene = nullptr;
	}

	dynamic_cast<GetTextBleRequestMethod*>(requestMethod)->AddOnReturn(callableObject, callback, name);
	return 0;
}
