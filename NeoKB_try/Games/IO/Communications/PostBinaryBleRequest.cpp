#include "PostBinaryBleRequest.h"


#include "BleAccess.h"
#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Framework/Scenes/SceneMaster.h"
#include "../../../Util/DataStructure/FileSegmentMap.h"
#include "../../../Util/StringSplitter.h"
#include "../../Output/Bluetooths/MeteoFileSegmentBluetoothMessage.h"
#include "../../Output/Bluetooths/MeteoAckFileSegmentBluetoothMessage.h"


using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;
using namespace Framework::Scenes;
using namespace Util::DataStructure;
using namespace Util;



PostBinaryBleRequest::PostBinaryBleRequestMethod::PostBinaryBleRequestMethod(string fPath,
	MeteoBluetoothMessage* gMessage,
	MeteoCommand ackGetCommand, 
	MeteoCommand tCommand, 
	MeteoCommand aTransferCommand,
	MeteoCommand fCommand, 
	MeteoCommand rRetransferCommand, 
	MeteoCommand aFinishCommand)
{
	filePath = fPath;


	transferCommand = tCommand;
	ackTransferCommand = aTransferCommand;
	finishCommand = fCommand;
	requestRetransferCommand = rRetransferCommand;
	ackFinishCommand = aFinishCommand;



}

int PostBinaryBleRequest::PostBinaryBleRequestMethod::PerformAndWait(BleRequest * thisRequest)
{
	// #-- 1 開始
	PostBinaryBleRequest* thisPostBinaryRequest = dynamic_cast<PostBinaryBleRequest*>(thisRequest);

	/* 抓目前的藍芽mtu */
	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisPostBinaryRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<BleAccess*>(thisPostBinaryRequest->communicationComponent)->GetPeripheral());


	bluetoothPhone->PushOutputMessage(postMessage);

	bool isAckReceived = false;

	while (!isAckReceived) {

		if (thisPostBinaryRequest->getElapsedSeconds() > thisPostBinaryRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisPostBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisPostBinaryRequest->rawMessageMutex);
		while (thisPostBinaryRequest->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisPostBinaryRequest->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackPostCommand) {

					/* 檢查Scene是否還存在，存在才能執行 */
					if ((thisPostBinaryRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisPostBinaryRequest->callbackScene)) ||
						!thisPostBinaryRequest->isCallbackByScene ||
						onAck.GetSize() == 0)
						onAck.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson());
					else {
						throw BleRequestException(BleResponseCode::Gone);
					}

					isAckReceived = true;

				}
			}
			thisPostBinaryRequest->inputRawMessages.pop_back();
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
	}

	/* 讀檔並輸入map中 */
	fstream file(filePath, ios::binary);

	FileSegmentMap bleBinaryRequestFileSegmentMap(binarySegmentSize);
	bleBinaryRequestFileSegmentMap.ReadFile(&file);
	vector<string> splitPath = StringSplitter::Split(filePath, "/");
	string fileName = splitPath[splitPath.size() - 1];
	bleBinaryRequestFileSegmentMap.fileName = fileName;

	// 接下來寫一個builder可以把file segment包成message丟給bluetooth phone，丟完以後再做後續動作如finish command、retransfer等等

	vector<MeteoFileSegmentBluetoothMessage*> fileSegmentMessages;

	for (int i = 0; i < bleBinaryRequestFileSegmentMap.segmentAmount; i++) {
		char* tempFileSegment = nullptr;
		int tempFileSegmentSize = 0;
		tempFileSegmentSize = bleBinaryRequestFileSegmentMap.GetFileSegment(i, &tempFileSegment);

		MeteoFileSegmentBluetoothMessage* fileSegmentMessage = new MeteoFileSegmentBluetoothMessage(
			transferCommand,
			tempFileSegment,
			tempFileSegmentSize,
			bleBinaryRequestFileSegmentMap.fileName,
			i,
			bleBinaryRequestFileSegmentMap.segmentAmount);

		delete[] tempFileSegment;

		fileSegmentMessages.push_back(fileSegmentMessage);
	}

	if (fileSegmentMessages.size() == 0) {
		LOG(LogLevel::Error) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : file segment resolve to no file segment.";
		throw BleRequestException(BleResponseCode::TransferToFileSegmentsFailed);
	}

	// #-- 2 丟檔

	/* 這邊要做丟檔案，每丟一個檔案以後要不停檢查是否接到Ack，才能再丟下一個檔案 */

	int tempSendFileSegmentNumber = 0;
	double transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
	bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[0]);


	while (1) {

		if (thisPostBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* 檢查Scene是否還存在，不存在就直接跳出 */
		if (thisPostBinaryRequest->isCallbackByScene && !SceneMaster::GetInstance().CheckScene(thisPostBinaryRequest->callbackScene))
		{
			throw BleRequestException(BleResponseCode::Gone);
		}

		// 如果太久沒收到ack，就直接丟下一個file segment
		if (thisPostBinaryRequest->getSectionElapsedSeconds() - transferTempFileSegmentElapsedSeconds > sendFileSegmentTimeout) {

			LOG(LogLevel::Warning) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : no receive ack file segment [" << tempSendFileSegmentNumber << "]. Pass.";

			// 已丟完所有file segment，就直接跳出往下一個步驟
			if (tempSendFileSegmentNumber == bleBinaryRequestFileSegmentMap.segmentAmount - 1)
				break;

			tempSendFileSegmentNumber++;
			transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
			bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[tempSendFileSegmentNumber]);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisPostBinaryRequest->rawMessageMutex);
		while (thisPostBinaryRequest->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisPostBinaryRequest->inputRawMessages.back();

			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)) {
				if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetCommand() == ackTransferCommand &&
					dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetFileName() == bleBinaryRequestFileSegmentMap.fileName) {

					MeteoAckFileSegmentBluetoothMessage* ackFileSegmentMessage = dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message);
					if (ackFileSegmentMessage->GetOrder() == tempSendFileSegmentNumber) {

						tempSendFileSegmentNumber++;
						transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
						bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[tempSendFileSegmentNumber]);
					}
					else if (ackFileSegmentMessage->GetOrder() < tempSendFileSegmentNumber) {
						// no-op
					}
					else {
						LOG(LogLevel::Error) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : ack file segment number worng -- [" << ackFileSegmentMessage->GetOrder() << "]. Temp file Segment Number is [" << tempSendFileSegmentNumber << "].";
					}
				}
			}
			thisPostBinaryRequest->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

	}
	
	// #-- 3 確認

	MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
	json messageContext;
	messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
	checkFinishMessage->SetContextInJson(messageContext);
	bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

	// 等待對方整理所有需要重傳的file segment
	this_thread::sleep_for(std::chrono::milliseconds(200));

	bool isFinished = false;
	bool isRetransferred = false;

	/* 從這個時間點開始計時，超過時間就timeout */
	thisPostBinaryRequest->writeTimePoint();

	// 目前重傳到 retransferOrder 中的第幾個 file segment
	int tempSendRetransferOrderIndex = -1;
	transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();

	while (1) {

		if (thisPostBinaryRequest->getSectionElapsedSeconds() > thisPostBinaryRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisPostBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		// 如果太久沒收到ack，就直接丟下一個file segment
		if (tempSendRetransferOrderIndex >= 0 &&
			thisPostBinaryRequest->getSectionElapsedSeconds() - transferTempFileSegmentElapsedSeconds > sendFileSegmentTimeout) {

			LOG(LogLevel::Warning) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : no receive ack retransfer file segment [" << tempSendRetransferOrderIndex << " / " << retransferOrders.size() << "] which file sgement is [" << retransferOrders[tempSendRetransferOrderIndex] << "]. Pass.";

			// 已丟完所有file segment，就直接跳出往下一個步驟
			if (tempSendRetransferOrderIndex == retransferOrders.size() - 1) {
				LOG(LogLevel::Debug) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : retransfer over.";
				bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

				// 等待對方整理所有需要重傳的file segment
				this_thread::sleep_for(std::chrono::milliseconds(200));
				transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
			}
			else {

				tempSendRetransferOrderIndex++;
				transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
				bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);

			}

		}

		/* 這段寫得很長，功能是檢查有沒有全都收到，有的話就離開，沒有就重傳，重傳完要再檢查一次有沒有收到 */
		unique_lock<mutex> uLock(thisPostBinaryRequest->rawMessageMutex);
		while (thisPostBinaryRequest->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisPostBinaryRequest->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackFinishCommand) {

				/* 檢查Scene是否還存在，存在才能執行 */
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["Status"].get<int>() == 0) {
					LOG(LogLevel::Debug) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : transfer success.";
					break;
				}

				LOG(LogLevel::Debug) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : transfer not success.";

			}
			else if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == requestRetransferCommand) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["FileName"].get<string>() == bleBinaryRequestFileSegmentMap.fileName) {
					int retransferOrder = dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["Order"].get<int>();

					retransferOrders.push_back(retransferOrder);

					if (retransferOrders.size() == 1) {
						tempSendRetransferOrderIndex = 0;
						bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);
					}

				}
			}}

			/* 街道ack以後再繼續重傳 */
			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)) {
				if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetCommand() == ackTransferCommand &&
					dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetFileName() == bleBinaryRequestFileSegmentMap.fileName) {

					int ackIndex = -1;
					for (int i = 0; i < retransferOrders.size(); i++) {
						if (retransferOrders[i] == dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetOrder()) {
							ackIndex = i;
						}
					}

					if (ackIndex != -1 && ackIndex != retransferOrders.size() - 1 && ackIndex == tempSendRetransferOrderIndex) {

						tempSendRetransferOrderIndex++;
						transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
						bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);

					}
					else if (ackIndex == retransferOrders.size() - 1) {
						bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

						// 等待對方整理所有需要重傳的file segment
						this_thread::sleep_for(std::chrono::milliseconds(200));

						transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
					}
					else if (ackIndex > tempSendRetransferOrderIndex) {
						LOG(LogLevel::Error) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : receive error ack retransfer index [" << ackIndex << "/" << retransferOrders.size() << "]. temp index is [" << tempSendRetransferOrderIndex << "]";
					}


				}
			}

			thisPostBinaryRequest->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// #-- 4

	/* 檢查Scene是否還存在，存在才能執行 */
	if ((thisPostBinaryRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisPostBinaryRequest->callbackScene)) ||
		!thisPostBinaryRequest->isCallbackByScene ||
		onFinish.GetSize() == 0)
		onFinish.TriggerThenClear();
	else if(!SceneMaster::GetInstance().CheckScene(thisPostBinaryRequest->callbackScene))
		throw BleRequestException(BleResponseCode::Gone);

	return 0;
}

BleRequestMethodType PostBinaryBleRequest::PostBinaryBleRequestMethod::GetMethodType()
{
	return BleRequestMethodType::PostBinary;
}

int PostBinaryBleRequest::PostBinaryBleRequestMethod::AddOnAck(MtoObject * callableObject, function<int(json)> callback, string name)
{
	onAck.Add(callableObject, callback, name);
	return 0;
}

int PostBinaryBleRequest::PostBinaryBleRequestMethod::AddOnFinish(MtoObject * callableObject, function<int()> callback, string name)
{
	onFinish.Add(callableObject, callback, name);
	return 0;
}

PostBinaryBleRequest::PostBinaryBleRequest(string fPath, MeteoBluetoothMessage * pMessage, MeteoCommand ackPostCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand) : RegisterType("PostBinaryBleRequest")
{
	PostBinaryBleRequestMethod* method = new PostBinaryBleRequestMethod(
		fPath,
		pMessage,
		ackPostCommand,
		tCommand,
		aTransferCommand,
		fCommand,
		rRetransferCommand,
		aFinishCommand
	);

	requestMethod = method;

}

int PostBinaryBleRequest::AddOnAck(MtoObject * callableObject, function<int(json)> callback, string name)
{
	/* 檢查是不是由scene去add的 */
	if (dynamic_cast<Scene*>(callableObject)) {
		isCallbackByScene &= (dynamic_cast<Scene*>(callableObject) != nullptr);
		if (isCallbackByScene)
			callbackScene = dynamic_cast<Scene*>(callableObject);
		else
			callbackScene = nullptr;
	}

	dynamic_cast<PostBinaryBleRequestMethod*>(requestMethod)->AddOnAck(callableObject, callback, name);
	return 0;
}

int PostBinaryBleRequest::AddOnFinish(MtoObject * callableObject, function<int()> callback, string name)
{
	/* 檢查是不是由scene去add的 */
	if (dynamic_cast<Scene*>(callableObject)) {
		isCallbackByScene &= (dynamic_cast<Scene*>(callableObject) != nullptr);
		if (isCallbackByScene)
			callbackScene = dynamic_cast<Scene*>(callableObject);
		else
			callbackScene = nullptr;
	}

	dynamic_cast<PostBinaryBleRequestMethod*>(requestMethod)->AddOnFinish(callableObject, callback, name);

	return 0;
}

int PostBinaryBleRequest::fail(exception * e)
{
	LOG(LogLevel::Error) << "PostBinaryBleRequest::fail() : not implemented.";
	return 0;
}
