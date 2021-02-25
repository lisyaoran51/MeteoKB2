#include "GetBinaryBleRequest.h"

#include "BleAccess.h"
#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Framework/Scenes/SceneMaster.h"
#include "../../../Util/DataStructure/FileSegmentMap.h"
#include "../../../Util/StringSplitter.h"
#include "../../Output/Bluetooths/MeteoFileSegmentBluetoothMessage.h"


using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;
using namespace Framework::Scenes;
using namespace Util::DataStructure;
using namespace Util;



GetBinaryBleRequest::GetBinaryBleRequestMethod::GetBinaryBleRequestMethod(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
{
	vector<string> filePathHierachy = StringSplitter::Split(fPath, "/");
	fileName = filePathHierachy[filePathHierachy.size() - 1];
	filePathHierachy.pop_back();
	directoryPath = StringSplitter::Combine(filePathHierachy, "/");

	getMessage = gMessage;
	ackGetCommand = aGetCommand;
	transferCommand = tCommand;
	finishCommand = fCommand;
	requestRetransferCommand = rRetransferCommand;
	ackFinishCommand = aFinishCommand;

}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait(BleRequest * thisRequest)
{

	GetBinaryBleRequest* thisGetBinaryRequest = dynamic_cast<GetBinaryBleRequest*>(thisRequest);

	/* 抓目前的藍芽mtu */
	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisGetBinaryRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<BleAccess*>(thisGetBinaryRequest->communicationComponent)->GetPeripheral());


	bluetoothPhone->PushOutputMessage(getMessage);

	bool isAckReceived = false;

	while (!isAckReceived) {

		if (thisGetBinaryRequest->getElapsedSeconds() > thisGetBinaryRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisGetBinaryRequest->rawMessageMutex);
		for (int i = 0; i < thisGetBinaryRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisGetBinaryRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisGetBinaryRequest->inputRawMessages[i])->GetCommand() == ackGetCommand) {

					/* 檢查Scene是否還存在，存在才能執行 */
					if (SceneMaster::GetInstance().CheckScene(thisGetBinaryRequest->callbackScene))
						onAck.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(thisGetBinaryRequest->inputRawMessages[i])->GetContextInJson());
					else {
						// fail
						BleRequestException except(BleResponseCode::Gone);
						thisGetBinaryRequest->Fail(except);
					}

					isAckReceived = true;

				}
			}
			delete thisGetBinaryRequest->inputRawMessages[i];
		}
		thisGetBinaryRequest->inputRawMessages.clear();

		uLock.unlock();
	}



	int mtu = bleAccess->GetMtu();

	int binarySegmentSize = mtu - 28;
	if (binarySegmentSize <= 0) {
		LOG(LogLevel::Error) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : mtu size " << mtu << " too small.";
		throw logic_error("BleRequest::PostBinaryBleRequestMethod::PerformAndWait(): mtu size too small.");
	}

	FileSegmentMap bleBinaryRequestFileSegmentMap(binarySegmentSize);


	/* 這邊開始收資料 */
	while (!isTransferFinished) {

		if (thisGetBinaryRequest->getElapsedSeconds() > thisGetBinaryRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* 確認收到的command是不是return command */
		unique_lock<mutex> uLock(thisGetBinaryRequest->rawMessageMutex);
		for (int i = 0; i < thisGetBinaryRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(thisGetBinaryRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(thisGetBinaryRequest->inputRawMessages[i])->GetCommand() == transferCommand) {

					MeteoFileSegmentBluetoothMessage* fileSegmentMessage = dynamic_cast<MeteoFileSegmentBluetoothMessage*>(thisGetBinaryRequest->inputRawMessages[i]);

					if (fileSegmentMessage->GetFileName() != fileName)
						continue;

					char* fileSegmentData = nullptr;
					int size = fileSegmentMessage->GetFileSegment(&fileSegmentData);
					int order = fileSegmentMessage->GetOrder();

					bleBinaryRequestFileSegmentMap.fileSegmentMap[order] = pair<char*, int>(fileSegmentData, size);

					int amount = fileSegmentMessage->GetAmount();

					if (bleBinaryRequestFileSegmentMap.fileSegmentMap.size() == amount) {
						isTransferFinished = true;
					}
				}
			}
			delete thisGetBinaryRequest->inputRawMessages[i];
		}
		thisGetBinaryRequest->inputRawMessages.clear();

		uLock.unlock();
	}

	// file segment map write file

	fstream file(directoryPath + string("/") + fileName, ios::out | ios::binary);

	bleBinaryRequestFileSegmentMap.WriteFile(&file);

	/* 告知已收到檔案 */
	while (!isAckedTransferFinished) {

		/* 確認收到的command是不是return command */
		unique_lock<mutex> uLock(thisGetBinaryRequest->rawMessageMutex);
		for (int i = 0; i < thisGetBinaryRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisGetBinaryRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisGetBinaryRequest->inputRawMessages[i])->GetCommand() == finishCommand) {

					MeteoContextBluetoothMessage* ackFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
					json messageContext;
					messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
					ackFinishMessage->SetContextInJson(messageContext);
					bleAccess->GetBluetoothPhone()->PushOutputMessage(ackFinishMessage);

					isAckedTransferFinished = true;
				}
			}
			delete thisGetBinaryRequest->inputRawMessages[i];
		}
		thisGetBinaryRequest->inputRawMessages.clear();

		uLock.unlock();

	}

	/* 檢查Scene是否還存在，存在才能執行 */
	if (SceneMaster::GetInstance().CheckScene(thisGetBinaryRequest->callbackScene))	// 之後要改掉這個，不需要驗證場景是否存在
		onFinish.TriggerThenClear(&bleBinaryRequestFileSegmentMap);



	return 0;
}

BleRequestMethodType GetBinaryBleRequest::GetBinaryBleRequestMethod::GetMethodType()
{
	return BleRequestMethodType::GetBinary;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::AddOnFinish(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name)
{
	onFinish.Add(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::AddOnFinish(ActionList<int, FileSegmentMap*>* actionsOnFinish)
{
	onFinish.Add(actionsOnFinish);
	return 0;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::AddOnGetBinarySuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequestMethod::AddOnGetBinarySuccess() : not implemented.";
	return 0;
}

GetBinaryBleRequest::GetBinaryBleRequest(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand ackGetCommand, MeteoCommand tCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
{
	LOG(LogLevel::Error) << "GetBinaryBleRequest::GetBinaryBleRequest() : not implemented.";
}

int GetBinaryBleRequest::ChooseCommunicationComponentAndPerform()
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequest::ChooseCommunicationComponentAndPerform() : not implemented.";
	return 0;
}

int GetBinaryBleRequest::AddOnFinish(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name)
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequest::AddOnFinish() : not implemented.";
	return 0;
}

int GetBinaryBleRequest::AddOnFinish(ActionList<int, FileSegmentMap*>* actionsOnFinish)
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequest::AddOnFinish() : not implemented.";
	return 0;
}

int GetBinaryBleRequest::AddOnGetBinarySuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequest::AddOnGetBinarySuccess() : not implemented.";
	return 0;
}

int GetBinaryBleRequest::AddOnGetBinarySuccess(ActionList<int, string>* actionsOnGetBinarySuccess)
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequest::AddOnGetBinarySuccess() : not implemented.";
	return 0;
}
