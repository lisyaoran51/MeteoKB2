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

		/* �T�{���쪺command�O���Oreturn command */
		unique_lock<mutex> uLock(thisGetTextRequest->rawMessageMutex);
		for (int i = 0; i < thisGetTextRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisGetTextRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisGetTextRequest->inputRawMessages[i])->GetCommand() == returnCommand) {

					// �T�{Scene�٦s���s�b�A�p�G���s�b�N������callback�A�קK��scene�w�g�Q�R�������p
					/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
					if (SceneMaster::GetInstance().CheckScene(thisGetTextRequest->callbackScene))
						onReturn.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(thisGetTextRequest->inputRawMessages[i])->GetContextInJson());

					return 0;

				}
			}
			delete thisGetTextRequest->inputRawMessages[i];
		}
		thisGetTextRequest->inputRawMessages.clear();

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));



		/* �o�q���O�g�o���n�A���ΤF
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
