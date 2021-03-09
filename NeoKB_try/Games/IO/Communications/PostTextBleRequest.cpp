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

			if (thisPostTextRequest->exitRequested) {
				throw BleRequestException(BleResponseCode::ExitRequested);
			}

			/* �o�q�g�o�ܪ��A�\��N�u�O�⦬�쪺ack�ᵹrequest�Ӥw */
			unique_lock<mutex> uLock(thisPostTextRequest->rawMessageMutex);
			while(thisPostTextRequest->inputRawMessages.size() > 0) {

				MeteoBluetoothMessage* message = thisPostTextRequest->inputRawMessages.back();

				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
					if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackCommand) {

						/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
						if ((thisPostTextRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisPostTextRequest->callbackScene)) ||
							!thisPostTextRequest->isCallbackByScene ||
							onAck.GetSize() == 0)
							onAck.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson());

						return 0;

					}
				}

				thisPostTextRequest->inputRawMessages.pop_back();
				delete message;
			}

			uLock.unlock();

			this_thread::sleep_for(std::chrono::milliseconds(100));

			/* �ª��{���A�g�o���n
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

int PostTextBleRequest::PostTextBleRequestMethod::AddOnAck(MtoObject * callableObject, function<int(json)> callback, string name)
{
	return onAck.Add(callableObject, callback, name);
}

PostTextBleRequest::PostTextBleRequest() : RegisterType("PostTextBleRequest")
{
	// �Ψ�override��
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

int PostTextBleRequest::AddOnAck(MtoObject * callableObject, function<int(json)> callback, string name)
{
	/* �ˬd�O���O��scene�hadd�� */
	if (dynamic_cast<Scene*>(callableObject)) {
		isCallbackByScene &= (dynamic_cast<Scene*>(callableObject) != nullptr);
		if (isCallbackByScene)
			callbackScene = dynamic_cast<Scene*>(callableObject);
		else
			callbackScene = nullptr;
	}

	dynamic_cast<PostTextBleRequestMethod*>(requestMethod)->AddOnAck(callableObject, callback, name);

	return 0;
}
