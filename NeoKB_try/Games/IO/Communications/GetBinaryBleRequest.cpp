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



GetBinaryBleRequest::GetBinaryBleRequestMethod::GetBinaryBleRequestMethod(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
{
	vector<string> filePathHierachy = StringSplitter::Split(fPath, "/");
	fileName = filePathHierachy[filePathHierachy.size() - 1];
	filePathHierachy.pop_back();
	directoryPath = StringSplitter::Combine(filePathHierachy, "/");

	getMessage = gMessage;
	ackGetCommand = aGetCommand;
	transferCommand = tCommand;
	ackTransferCommand = aTransferCommand;
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

		if (thisGetBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisGetBinaryRequest->rawMessageMutex);
		while(thisGetBinaryRequest->inputRawMessages.size() > 0) {

			MeteoBluetoothMessage* message = thisGetBinaryRequest->inputRawMessages.back();
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackGetCommand) {

					/* 檢查Scene是否還存在，存在才能執行 */
					//if ((thisGetBinaryRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryRequest->callbackScene)) ||
					//	!thisGetBinaryRequest->isCallbackByScene)
					//	onAck.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson());
					//else {
					//	throw BleRequestException(BleResponseCode::Gone);
					//}

					isAckReceived = true;

				}
			}

			thisGetBinaryRequest->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));

	}



	int mtu = bleAccess->GetMtu();

	int binarySegmentSize = mtu - 28;
	if (binarySegmentSize <= 0) {
		LOG(LogLevel::Error) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : mtu size " << mtu << " too small.";

		throw BleRequestException(BleResponseCode::MtuTooSmall);
		//throw logic_error("BleRequest::PostBinaryBleRequestMethod::PerformAndWait(): mtu size too small.");
		// TODO: 應該中斷連線並報錯，不應該直接當掉
	}

	FileSegmentMap bleBinaryRequestFileSegmentMap(binarySegmentSize);
	bleBinaryRequestFileSegmentMap.segmentAmount = 100;	// 預設一個數，之後會再補上真實大小

	/* 從這個時間點開始計時，超過時間就timeout */
	thisGetBinaryRequest->writeTimePoint();

	/* 這邊開始收資料 */
	while (!isTransferFinished) {

		if (thisGetBinaryRequest->getSectionElapsedSeconds() > thisGetBinaryRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisGetBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* 確認收到的command是不是return command */
		unique_lock<mutex> uLock(thisGetBinaryRequest->rawMessageMutex);
		while (thisGetBinaryRequest->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisGetBinaryRequest->inputRawMessages.back();

			if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(message)->GetCommand() == transferCommand) {

				MeteoFileSegmentBluetoothMessage* fileSegmentMessage = dynamic_cast<MeteoFileSegmentBluetoothMessage*>(message);

				if (fileSegmentMessage->GetFileName() != fileName)
					continue;

				char* fileSegmentData = nullptr;
				int size = fileSegmentMessage->GetFileSegment(&fileSegmentData);
				int order = fileSegmentMessage->GetOrder();

				bleBinaryRequestFileSegmentMap.fileSegmentMap[order] = pair<char*, int>(fileSegmentData, size);

				int amount = fileSegmentMessage->GetAmount();
				bleBinaryRequestFileSegmentMap.segmentAmount = amount;

				if (bleBinaryRequestFileSegmentMap.fileSegmentMap.size() == amount) {
					isTransferFinished = true;
				}

				/* 從這個時間點開始計時，超過時間就timeout */
				thisGetBinaryRequest->writeTimePoint();

			}
			}
			else if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == finishCommand) {
				MeteoContextBluetoothMessage* reuqestRetransferMessage = new MeteoContextBluetoothMessage(requestRetransferCommand);

				json messageContext;
				messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;

				for (int i = 0; i < bleBinaryRequestFileSegmentMap.segmentAmount; i++) {
					map<int, pair<char*, int>>::iterator iter;
					iter = bleBinaryRequestFileSegmentMap.fileSegmentMap.find(i);
					if (iter == bleBinaryRequestFileSegmentMap.fileSegmentMap.end()) {
						messageContext["SegmentNumber"].push_back(i);
						if (messageContext["SegmentNumber"].size() > 20)
							break;	// 如果太多內容，會超過封包大小

					}
				}

				reuqestRetransferMessage->SetContextInJson(messageContext);
				reuqestRetransferMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
				bleAccess->GetBluetoothPhone()->PushOutputMessage(reuqestRetransferMessage);

				/* 從這個時間點開始計時，超過時間就timeout */
				thisGetBinaryRequest->writeTimePoint();

			}
			}

			thisGetBinaryRequest->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// file segment map write file

	fstream file(directoryPath + string("/") + fileName, ios::out | ios::binary);

	bleBinaryRequestFileSegmentMap.WriteFile(&file);

	/* 告知已收到檔案 */
	while (!isAckedTransferFinished) {

		if (thisGetBinaryRequest->getSectionElapsedSeconds() > thisGetBinaryRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisGetBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* 確認收到的command是不是return command */
		unique_lock<mutex> uLock(thisGetBinaryRequest->rawMessageMutex);
		while (thisGetBinaryRequest->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisGetBinaryRequest->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == finishCommand) {

					MeteoContextBluetoothMessage* ackFinishMessage = new MeteoContextBluetoothMessage(ackFinishCommand);
					json messageContext;
					messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
					ackFinishMessage->SetContextInJson(messageContext);
					ackFinishMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
					bleAccess->GetBluetoothPhone()->PushOutputMessage(ackFinishMessage);

					isAckedTransferFinished = true;

					/* 從這個時間點開始計時，超過時間就timeout */
					thisGetBinaryRequest->writeTimePoint();
				}
			}
			thisGetBinaryRequest->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::Fail(BleRequest * thisRequest)
{
	LOG(LogLevel::Debug) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::Fail() : get file failed.";

	if (fileSegmentMap == nullptr)
		return 0;

	GetBinaryBleRequest* thisGetBinaryBleRequest = dynamic_cast<GetBinaryBleRequest*>(thisRequest);
	if ((thisGetBinaryBleRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequest->callbackScene)) ||
		!thisGetBinaryBleRequest->isCallbackByScene)
		onFail.TriggerThenClear(fileSegmentMap);

	return 0;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::Success(BleRequest * thisRequest)
{
	LOG(LogLevel::Debug) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::Fail() : get file success.";

	if (fileSegmentMap == nullptr)
		return 0;

	GetBinaryBleRequest* thisGetBinaryBleRequest = dynamic_cast<GetBinaryBleRequest*>(thisRequest);
	if ((thisGetBinaryBleRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequest->callbackScene)) ||
		!thisGetBinaryBleRequest->isCallbackByScene)
		onSuccess.TriggerThenClear(fileSegmentMap);

	return 0;
}

BleRequestMethodType GetBinaryBleRequest::GetBinaryBleRequestMethod::GetMethodType()
{
	return BleRequestMethodType::GetBinary;
}

GetBinaryBleRequest::GetBinaryBleRequest(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand ackGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand) : RegisterType("GetBinaryBleRequest")
{
	LOG(LogLevel::Error) << "GetBinaryBleRequest::GetBinaryBleRequest() : not implemented.";
}

int GetBinaryBleRequest::ChooseCommunicationComponentToPerform()
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequest::ChooseCommunicationComponentToPerform() : not implemented.";
	return 0;
}

int GetBinaryBleRequest::AddOnSuccess(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name)
{
	dynamic_cast<GetBinaryBleRequestMethod*>(requestMethod)->AddOnSuccess(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequest::AddOnSuccess(ActionList<int, FileSegmentMap*>* callback)
{
	dynamic_cast<GetBinaryBleRequestMethod*>(requestMethod)->AddOnSuccess(callback);
	return 0;
}

int GetBinaryBleRequest::AddOnFail(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name)
{
	dynamic_cast<GetBinaryBleRequestMethod*>(requestMethod)->AddOnFail(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequest::AddOnFail(ActionList<int, FileSegmentMap*>* callback)
{
	dynamic_cast<GetBinaryBleRequestMethod*>(requestMethod)->AddOnFail(callback);
	return 0;
}

int GetBinaryBleRequest::fail(exception * e)
{
	requestMethod->Fail(this);
	return 0;
}

int GetBinaryBleRequest::success()
{
	requestMethod->Success(this);
	return 0;
}
