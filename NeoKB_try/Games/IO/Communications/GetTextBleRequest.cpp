#include "GetTextBleRequest.h"

#include "ForegroundBleAccess.h"
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

	ForegroundBleAccess* bleAccess = dynamic_cast<ForegroundBleAccess*>(thisGetTextRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<ForegroundBleAccess*>(thisGetTextRequest->communicationComponent)->GetPeripheral());

	MeteoContextBluetoothMessage* outputMessage = nullptr;

	bluetoothPhone->PushOutputMessage(getMessage);

	bool isReceived = false;

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

					returnText = dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContext();

					isReceived = true;

				}
			}
			thisGetTextRequest->inputRawMessages.pop_back();
			delete message;
		}
		uLock.unlock();

		if (isReceived)
			break;

		this_thread::sleep_for(std::chrono::milliseconds(100));


	}




	return 0;
}

BleRequestMethodType GetTextBleRequest::GetTextBleRequestMethod::GetMethodType()
{
	return BleRequestMethodType::GetText;
}

int GetTextBleRequest::GetTextBleRequestMethod::Fail(BleRequest * thisRequest)
{
	LOG(LogLevel::Debug) << "GetTextBleRequest::GetTextBleRequestMethod::Fail() : get text failed.";

	GetTextBleRequest* thisGetTextBleRequest = dynamic_cast<GetTextBleRequest*>(thisRequest);
	if ((thisGetTextBleRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetTextBleRequest->callbackScene)) ||
		!thisGetTextBleRequest->isCallbackByScene)
		onFail.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(getMessage)->GetContextInJson());

	return 0;
}

int GetTextBleRequest::GetTextBleRequestMethod::Success(BleRequest * thisRequest)
{
	LOG(LogLevel::Debug) << "GetTextBleRequest::GetTextBleRequestMethod::Fail() : get text success.";

	GetTextBleRequest* thisGetTextBleRequest = dynamic_cast<GetTextBleRequest*>(thisRequest);
	if ((thisGetTextBleRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetTextBleRequest->callbackScene)) ||
		!thisGetTextBleRequest->isCallbackByScene)
		onFail.TriggerThenClear(GetReturnJson());

	return 0;
}

string GetTextBleRequest::GetTextBleRequestMethod::GetReturnText()
{
	return returnText;
}

json GetTextBleRequest::GetTextBleRequestMethod::GetReturnJson()
{
	if (returnText == "{}")
		return json();
	// TODO: 解析錯誤的時候丟例外

	return json(returnText);
}

int GetTextBleRequest::GetTextBleRequestMethod::AddOnFail(MtoObject * callableObject, function<int(json)> callback, string name)
{
	onFail.Add(callableObject, callback, name);
	return 0;
}

int GetTextBleRequest::GetTextBleRequestMethod::AddOnSuccess(MtoObject * callableObject, function<int(json)> callback, string name)
{
	onSuccess.Add(callableObject, callback, name);
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

int GetTextBleRequest::AddOnFail(MtoObject * callableObject, function<int(json)> callback, string name)
{
	dynamic_cast<GetTextBleRequestMethod*>(requestMethod)->AddOnFail(callableObject, callback, name);
	return 0;
}

int GetTextBleRequest::AddOnSuccess(MtoObject * callableObject, function<int(json)> callback, string name)
{
	dynamic_cast<GetTextBleRequestMethod*>(requestMethod)->AddOnSuccess(callableObject, callback, name);
	return 0;
}

int GetTextBleRequest::fail(exception * e)
{
	requestMethod->Fail(this);
	return 0;
}

int GetTextBleRequest::success()
{
	requestMethod->Success(this);
	return 0;
}