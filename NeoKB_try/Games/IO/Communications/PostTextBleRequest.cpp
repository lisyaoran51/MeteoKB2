#include "PostTextBleRequest.h"

#include "ForegroundBleAccess.h"
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

	ForegroundBleAccess* bleAccess = dynamic_cast<ForegroundBleAccess*>(thisPostTextRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<ForegroundBleAccess*>(thisPostTextRequest->communicationComponent)->GetPeripheral());

	MeteoContextBluetoothMessage* outputMessage = nullptr;
	// TODO: command to message
	bluetoothPhone->PushOutputMessage(outputMessage);

	if (!isNeedCheckAck)
		return 0;

	bool isReceived = false;

	while (1) {

		if (thisPostTextRequest->getElapsedSeconds() > thisPostTextRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisPostTextRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisPostTextRequest->rawMessageMutex);
		while(thisPostTextRequest->inputRawMessages.size() > 0) {

			MeteoBluetoothMessage* message = thisPostTextRequest->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackCommand) {


					ackText = dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContext();

					isReceived = true;
				}
			}

			thisPostTextRequest->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		if (isReceived)
			break;

		this_thread::sleep_for(std::chrono::milliseconds(100));

	}


	return 0;
}

BleRequestMethodType PostTextBleRequest::PostTextBleRequestMethod::GetMethodType()
{
	return BleRequestMethodType::PostText;
}

int PostTextBleRequest::PostTextBleRequestMethod::Fail(BleRequest * thisRequest)
{
	LOG(LogLevel::Debug) << "PostTextBleRequest::PostTextBleRequestMethod::Fail() : post text failed.";

	PostTextBleRequest* thisPostTextBleRequest = dynamic_cast<PostTextBleRequest*>(thisRequest);
	if ((thisPostTextBleRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisPostTextBleRequest->callbackScene)) ||
		!thisPostTextBleRequest->isCallbackByScene)
		onFail.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(postMessage)->GetContextInJson());

	return 0;
}

int PostTextBleRequest::PostTextBleRequestMethod::Success(BleRequest * thisRequest)
{
	LOG(LogLevel::Debug) << "PostTextBleRequest::PostTextBleRequestMethod::Success() : post text success.";

	if (!isNeedCheckAck)
		return 0;

	PostTextBleRequest* thisPostTextBleRequest = dynamic_cast<PostTextBleRequest*>(thisRequest);
	if ((thisPostTextBleRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisPostTextBleRequest->callbackScene)) ||
		!thisPostTextBleRequest->isCallbackByScene)
		onSuccess.TriggerThenClear(GetAckJson());

	return 0;
}

string PostTextBleRequest::PostTextBleRequestMethod::GetAckText()
{
	return ackText;
}

json PostTextBleRequest::PostTextBleRequestMethod::GetAckJson()
{
	if (ackText == "{}")
		return json();
	// TODO: 解析錯誤的時候丟例外
	return json(ackText);
}

int PostTextBleRequest::PostTextBleRequestMethod::AddOnFail(MtoObject * callableObject, function<int(json)> callback, string name)
{
	onFail.Add(callableObject, callback, name);

	return 0;
}

int PostTextBleRequest::PostTextBleRequestMethod::AddOnSuccess(MtoObject * callableObject, function<int(json)> callback, string name)
{
	onSuccess.Add(callableObject, callback, name);
	return 0;
}

PostTextBleRequest::PostTextBleRequest() : RegisterType("PostTextBleRequest")
{
	// 用來override的
}

PostTextBleRequest::PostTextBleRequest(MeteoBluetoothMessage * pMessage) : RegisterType("PostTextBleRequest")
{
	PostTextBleRequestMethod* method = new PostTextBleRequestMethod(pMessage);
	requestMethod = method;
}

PostTextBleRequest::PostTextBleRequest(MeteoBluetoothMessage * pMessage, MeteoCommand aCommand) : RegisterType("PostTextBleRequest")
{
	PostTextBleRequestMethod* method = new PostTextBleRequestMethod(pMessage, aCommand);
	requestMethod = method;
}

int PostTextBleRequest::AddOnFail(MtoObject * callableObject, function<int(json)> callback, string name)
{
	dynamic_cast<PostTextBleRequestMethod*>(requestMethod)->AddOnFail(callableObject, callback, name);
	return 0;
}

int PostTextBleRequest::AddOnSuccess(MtoObject * callableObject, function<int(json)> callback, string name)
{
	dynamic_cast<PostTextBleRequestMethod*>(requestMethod)->AddOnSuccess(callableObject, callback, name);
	return 0;
}

int PostTextBleRequest::fail(exception * e)
{
	requestMethod->Fail(this);
	return 0;
}

int PostTextBleRequest::success()
{
	requestMethod->Success(this);
	return 0;
}
