#include "BleRequest.h"

#include "BleAccess.h"
#include "../../Input/Commands/MeteoTextBluetoothCommand.h"


using namespace Games::IO::Communications;
using namespace Games::Input::Commands;


BleRequestException::BleRequestException(BleResponseCode bResponseCode)
{
	bleResponseCode = bResponseCode;
}

BleResponseCode BleRequestException::GetBleResponseCode()
{
	return BleResponseCode();
}

int BleRequest::Perform(CommunicationComponent * cComponent)
{
	communicationComponent = cComponent;

	requestStartTime = system_clock::now();

	// preform ���T�X�h�M�ᵥ�^��

	requestMethod->PerformAndWait(this);

	// �p�G���X�{���~�A�|��exception�A�N���|����on success

	communicationComponent->GetScheduler()->AddTask([=]() {
		onSuccess.TriggerThenClear();
		return 0;
	});

	return 0;
}


BleRequest::PostTextBleRequestMethod::PostTextBleRequestMethod(MeteoBluetoothCommand* pMessage) : postMessage(pMessage)
{
	isNeedCheckAck = false;
}

BleRequest::PostTextBleRequestMethod::PostTextBleRequestMethod(MeteoBluetoothCommand* pMessage, MeteoCommand aCommand) : postMessage(pMessage)
{
	ackCommand = aCommand;
}

int BleRequest::PostTextBleRequestMethod::PerformAndWait(BleRequest* thisRequest)
{
	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<BleAccess*>(thisRequest->communicationComponent)->GetPeripheral());

	bluetoothPhone->PushMessage(postMessage);

	if (isNeedCheckAck) {

		while (1) {

			if (thisRequest->getElapsedSeconds() > thisRequest->timeout) {
				throw BleRequestException(BleResponseCode::RequestTimeout);
			}

			/* �o�q�g�o�ܪ��A�\��N�u�O�⦬�쪺ack�ᵹrequest�Ӥw */
			for (int i = 0; i < bleAccess->GetInputRawCommand().size(); i++) {
				if (dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()[i])) {
					if (dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()[i])->GetCommand() == ackCommand) {

						onAck.TriggerThenClear(dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()[i])->GetContextInJson());

						bleAccess->GetInputRawCommand().clear();
						return 0;

					}
				}
			}

			bleAccess->GetInputRawCommand().clear();
		}

	}


	return 0;
}

int BleRequest::PostTextBleRequestMethod::AddOnAck(MtoObject * callableObject, function<int(json)> callback, string name)
{
	return onAck.Add(callableObject, callback, name);
}
