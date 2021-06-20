#include "GetBinaryBleRequestHandler.h"

#include "ForegroundBleAccess.h"
#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Output/Bluetooths/MeteoFileSegmentBluetoothMessage.h"
#include "../../Output/Bluetooths/MeteoAckFileSegmentBluetoothMessage.h"
#include "../../../Framework/Scenes/SceneMaster.h"
#include "../../../Util/DataStructure/FileSegmentMap.h"
#include "../../../Util/StringSplitter.h"
#include <experimental/filesystem>





using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;
using namespace Framework::Scenes;
using namespace Util::DataStructure;
using namespace Util;
using namespace std::experimental::filesystem;




GetBinaryBleRequestHandler::GetBinaryBleRequestHandler(string dPath, string fName, MeteoCommand gCommand, MeteoCommand ackGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand) : RegisterType("GetBinaryBleRequestHandler")
{
	GetBinaryBleRequestHandlerMethod* method = new GetBinaryBleRequestHandlerMethod(
		dPath,
		fName,
		gCommand,
		ackGetCommand,
		tCommand,
		aTransferCommand,
		fCommand,
		rRetransferCommand,
		aFinishCommand
	);

	requestMethod = method;


}

int GetBinaryBleRequestHandler::AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	dynamic_cast<GetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnSuccess(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequestHandler::AddOnSuccess(ActionList<int, string>* callback)
{
	dynamic_cast<GetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnSuccess(callback);
	return 0;
}

int GetBinaryBleRequestHandler::AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name)
{
	dynamic_cast<GetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnFail(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequestHandler::AddOnFail(ActionList<int, string>* callback)
{
	dynamic_cast<GetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnFail(callback);
	return 0;
}

int GetBinaryBleRequestHandler::fail(exception * e)
{
	requestMethod->Fail(this);
	return 0;
}

int GetBinaryBleRequestHandler::success()
{
	requestMethod->Success(this);
	return 0;
}

GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::GetBinaryBleRequestHandlerMethod(string dPath, string fName, MeteoCommand gCommand, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
{
	directoryPath = dPath;
	fileName = fName;
	getCommand = gCommand; 
	ackGetCommand = aGetCommand;
	transferCommand = tCommand;
	ackTransferCommand = aTransferCommand;
	finishCommand = fCommand;
	requestRetransferCommand = rRetransferCommand; 
	ackFinishCommand = aFinishCommand;

}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait(BleRequest * thisRequest)
{
	// #-- 1 開始
	GetBinaryBleRequestHandler* thisGetBinaryBleRequestHandler = dynamic_cast<GetBinaryBleRequestHandler*>(thisRequest);

	/* 抓目前的藍芽mtu */
	ForegroundBleAccess* bleAccess = dynamic_cast<ForegroundBleAccess*>(thisGetBinaryBleRequestHandler->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<ForegroundBleAccess*>(thisGetBinaryBleRequestHandler->communicationComponent)->GetPeripheral());


	bool isRequestReceived = false;

	while (!isRequestReceived) {

		if (thisGetBinaryBleRequestHandler->getElapsedSeconds() > thisGetBinaryBleRequestHandler->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisGetBinaryBleRequestHandler->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisGetBinaryBleRequestHandler->rawMessageMutex);
		while (thisGetBinaryBleRequestHandler->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisGetBinaryBleRequestHandler->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == getCommand) {

				/* 查看是否有檔案 */
				string requestFileName = dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["FileName"].get<string>();

				if (requestFileName != fileName)
					continue;

				MeteoContextBluetoothMessage* ackMessage = new MeteoContextBluetoothMessage(ackGetCommand);
				json messageContext;
				messageContext["FileName"] = fileName;

				if (exists(directoryPath + "/" + fileName)) {
					messageContext["Status"] = 0;
					ackMessage->SetContextInJson(messageContext);
					bleAccess->GetBluetoothPhone()->PushOutputMessage(ackMessage);
				}
				else {
					messageContext["Status"] = -1;
					ackMessage->SetContextInJson(messageContext);
					bleAccess->GetBluetoothPhone()->PushOutputMessage(ackMessage);

					throw BleRequestException(BleResponseCode::FileNotFound);
				}

				isRequestReceived = true;

			}
			}
			thisGetBinaryBleRequestHandler->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(10));

	}

	int mtu = bleAccess->GetMtu();

	int binarySegmentSize = mtu - 28;
	if (binarySegmentSize <= 0) {
		LOG(LogLevel::Error) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : mtu size " << mtu << " too small.";
		throw BleRequestException(BleResponseCode::MtuTooSmall);
		//throw logic_error("BleRequest::PostBinaryBleRequestMethod::PerformAndWait(): mtu size too small.");
	}

	/* 讀檔並輸入map中 */
	fstream file(directoryPath + string("/") + fileName, ios::binary);

	FileSegmentMap fileSegmentMap(binarySegmentSize);
	fileSegmentMap.ReadFile(&file);
	fileSegmentMap.fileName = fileName;

	// 接下來寫一個builder可以把file segment包成message丟給bluetooth phone，丟完以後再做後續動作如finish command、retransfer等等

	vector<MeteoFileSegmentBluetoothMessage*> fileSegmentMessages;

	for (int i = 0; i < fileSegmentMap.segmentAmount; i++) {
		char* tempFileSegment = nullptr;
		int tempFileSegmentSize = 0;
		tempFileSegmentSize = fileSegmentMap.GetFileSegment(i, &tempFileSegment);

		MeteoFileSegmentBluetoothMessage* fileSegmentMessage = new MeteoFileSegmentBluetoothMessage(
			transferCommand,
			tempFileSegment,
			tempFileSegmentSize,
			fileSegmentMap.fileName,
			i,
			fileSegmentMap.segmentAmount);

		delete[] tempFileSegment;

		fileSegmentMessages.push_back(fileSegmentMessage);
	}

	if (fileSegmentMessages.size() == 0) {
		LOG(LogLevel::Error) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : file segment resolve to no file segment.";
		throw BleRequestException(BleResponseCode::TransferToFileSegmentsFailed);
	}

	// #-- 2 丟檔

	/* 這邊要做丟檔案，每丟一個檔案以後要不停檢查是否接到Ack，才能再丟下一個檔案 */

	int tempSendFileSegmentNumber = 0;
	double transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
	bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[0]);


	while (1) {

		if (thisGetBinaryBleRequestHandler->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* 檢查Scene是否還存在，不存在就直接跳出 */
		if (thisGetBinaryBleRequestHandler->isCallbackByScene && !SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequestHandler->callbackScene))
		{
			throw BleRequestException(BleResponseCode::Gone);
		}

		// 如果太久沒收到ack，就直接丟下一個file segment
		if (thisGetBinaryBleRequestHandler->getSectionElapsedSeconds() - transferTempFileSegmentElapsedSeconds > sendFileSegmentTimeout) {

			LOG(LogLevel::Warning) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : no receive ack file segment [" << tempSendFileSegmentNumber << "]. Pass.";

			// 已丟完所有file segment，就直接跳出往下一個步驟
			if (tempSendFileSegmentNumber == fileSegmentMap.segmentAmount - 1)
				break;

			tempSendFileSegmentNumber++;
			transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
			bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[tempSendFileSegmentNumber]);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisGetBinaryBleRequestHandler->rawMessageMutex);
		while (thisGetBinaryBleRequestHandler->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisGetBinaryBleRequestHandler->inputRawMessages.back();

			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetCommand() == ackTransferCommand &&
				dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetFileName() == fileSegmentMap.fileName) {

				MeteoAckFileSegmentBluetoothMessage* ackFileSegmentMessage = dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message);
				if (ackFileSegmentMessage->GetOrder() == tempSendFileSegmentNumber) {

					tempSendFileSegmentNumber++;
					transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
					bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[tempSendFileSegmentNumber]);
				}
				else if (ackFileSegmentMessage->GetOrder() < tempSendFileSegmentNumber) {
					// no-op
				}
				else {
					LOG(LogLevel::Error) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : ack file segment number worng -- [" << ackFileSegmentMessage->GetOrder() << "]. Temp file Segment Number is [" << tempSendFileSegmentNumber << "].";
				}
			}
			}
			thisGetBinaryBleRequestHandler->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

	}

	// #-- 3 確認

	MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
	json messageContext;
	messageContext["FileName"] = fileSegmentMap.fileName;
	checkFinishMessage->SetContextInJson(messageContext);
	bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

	// 等待對方整理所有需要重傳的file segment
	this_thread::sleep_for(std::chrono::milliseconds(200));

	bool isFinished = false;
	bool isRetransferred = false;

	/* 從這個時間點開始計時，超過時間就timeout */
	thisGetBinaryBleRequestHandler->writeTimePoint();

	// 目前重傳到 retransferOrder 中的第幾個 file segment
	int tempSendRetransferOrderIndex = -1;
	transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();

	while (1) {

		if (thisGetBinaryBleRequestHandler->getSectionElapsedSeconds() > thisGetBinaryBleRequestHandler->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisGetBinaryBleRequestHandler->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		// 如果太久沒收到ack，就直接丟下一個file segment
		if (tempSendRetransferOrderIndex >= 0 &&
			thisGetBinaryBleRequestHandler->getSectionElapsedSeconds() - transferTempFileSegmentElapsedSeconds > sendFileSegmentTimeout) {

			LOG(LogLevel::Warning) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : no receive ack retransfer file segment [" << tempSendRetransferOrderIndex << " / " << retransferOrders.size() << "] which file sgement is [" << retransferOrders[tempSendRetransferOrderIndex] << "]. Pass.";

			// 已丟完所有file segment，就直接跳出往下一個步驟
			if (tempSendRetransferOrderIndex == retransferOrders.size() - 1) {
				LOG(LogLevel::Debug) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : retransfer over.";
				bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

				// 等待對方整理所有需要重傳的file segment
				this_thread::sleep_for(std::chrono::milliseconds(200));
				transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
			}
			else {

				tempSendRetransferOrderIndex++;
				transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
				bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);

			}

		}

		/* 這段寫得很長，功能是檢查有沒有全都收到，有的話就離開，沒有就重傳，重傳完要再檢查一次有沒有收到 */
		unique_lock<mutex> uLock(thisGetBinaryBleRequestHandler->rawMessageMutex);
		while (thisGetBinaryBleRequestHandler->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisGetBinaryBleRequestHandler->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackFinishCommand) {

				/* 檢查Scene是否還存在，存在才能執行 */
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["Status"].get<int>() == 0) {
					LOG(LogLevel::Debug) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : transfer success.";
					break;
				}

				LOG(LogLevel::Debug) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : transfer not success.";

			}
			else if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == requestRetransferCommand) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["FileName"].get<string>() == fileSegmentMap.fileName) {
				int retransferOrder = dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["Order"].get<int>();

				retransferOrders.push_back(retransferOrder);

				if (retransferOrders.size() == 1) {
					tempSendRetransferOrderIndex = 0;
					bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);
				}

			}
			}
			}

			/* 街道ack以後再繼續重傳 */
			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetCommand() == ackTransferCommand &&
				dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetFileName() == fileSegmentMap.fileName) {

				int ackIndex = -1;
				for (int i = 0; i < retransferOrders.size(); i++) {
					if (retransferOrders[i] == dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetOrder()) {
						ackIndex = i;
					}
				}

				if (ackIndex != -1 && ackIndex != retransferOrders.size() - 1 && ackIndex == tempSendRetransferOrderIndex) {

					tempSendRetransferOrderIndex++;
					transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
					bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);

				}
				else if (ackIndex == retransferOrders.size() - 1) {
					bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

					// 等待對方整理所有需要重傳的file segment
					this_thread::sleep_for(std::chrono::milliseconds(200));

					transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
				}
				else if (ackIndex > tempSendRetransferOrderIndex) {
					LOG(LogLevel::Error) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : receive error ack retransfer index [" << ackIndex << "/" << retransferOrders.size() << "]. temp index is [" << tempSendRetransferOrderIndex << "]";
				}
			}
			}

			thisGetBinaryBleRequestHandler->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// 結束

	return 0;

}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::Fail(BleRequest* thisRequest)
{
	LOG(LogLevel::Debug) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::Fail() : fail to handle get file request.";

	//if (fileName == "")
	//	return 0;

	GetBinaryBleRequestHandler* thisGetBinaryBleRequestHandler = dynamic_cast<GetBinaryBleRequestHandler*>(thisRequest);
	if ((thisGetBinaryBleRequestHandler->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequestHandler->callbackScene)) ||
		!thisGetBinaryBleRequestHandler->isCallbackByScene)
		onFail.TriggerThenClear(fileName);

	return 0;
}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::Success(BleRequest* thisRequest)
{
	LOG(LogLevel::Debug) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::Fail() : handle get file request success.";

	//if (fileName == "")
	//	return 0;

	GetBinaryBleRequestHandler* thisGetBinaryBleRequestHandler = dynamic_cast<GetBinaryBleRequestHandler*>(thisRequest);
	if ((thisGetBinaryBleRequestHandler->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequestHandler->callbackScene)) ||
		!thisGetBinaryBleRequestHandler->isCallbackByScene)
		onSuccess.TriggerThenClear(fileName);
	return 0;
}

BleRequestMethodType GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::GetMethodType()
{
	return BleRequestMethodType::GetBinaryHandler;
}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name)
{
	onFail.Add(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::AddOnFail(ActionList<int, string>* callback)
{
	onFail.Add(callback);
	return 0;
}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	onSuccess.Add(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::AddOnSuccess(ActionList<int, string>* callback)
{
	onSuccess.Add(callback);
	return 0;
}
