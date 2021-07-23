#include "BlockingGetBinaryBleRequestHandler.h"

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




BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandler(string dPath, string fName, MeteoCommand gCommand, MeteoCommand ackGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand) : RegisterType("BlockingGetBinaryBleRequestHandler")
{
	BlockingGetBinaryBleRequestHandlerMethod* method = new BlockingGetBinaryBleRequestHandlerMethod(
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

int BlockingGetBinaryBleRequestHandler::AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	dynamic_cast<BlockingGetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnSuccess(callableObject, callback, name);
	return 0;
}

int BlockingGetBinaryBleRequestHandler::AddOnSuccess(ActionList<int, string>* callback)
{
	dynamic_cast<BlockingGetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnSuccess(callback);
	return 0;
}

int BlockingGetBinaryBleRequestHandler::AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name)
{
	dynamic_cast<BlockingGetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnFail(callableObject, callback, name);
	return 0;
}

int BlockingGetBinaryBleRequestHandler::AddOnFail(ActionList<int, string>* callback)
{
	dynamic_cast<BlockingGetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnFail(callback);
	return 0;
}

int BlockingGetBinaryBleRequestHandler::fail(exception * e)
{
	requestMethod->Fail(this);
	return 0;
}

int BlockingGetBinaryBleRequestHandler::success()
{
	requestMethod->Success(this);
	return 0;
}

BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::BlockingGetBinaryBleRequestHandlerMethod(string dPath, string fName, MeteoCommand gCommand, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
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

	fileSegmentAckTimeout = 0.5;
}

int BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait(BleRequest * thisRequest)
{

	LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : start handler";

	// #-- 1 開始
	BlockingGetBinaryBleRequestHandler* thisGetBinaryBleRequestHandler = dynamic_cast<BlockingGetBinaryBleRequestHandler*>(thisRequest);

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

				LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : get request with filename." << requestFileName;

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

		this_thread::sleep_for(std::chrono::milliseconds(1));

	}

	int mtu = bleAccess->GetMtu();

	int binarySegmentSize = mtu - 28;
	if (binarySegmentSize <= 0) {
		LOG(LogLevel::Error) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : mtu size " << mtu << " too small.";
		throw BleRequestException(BleResponseCode::MtuTooSmall);
		//throw logic_error("BleRequest::PostBinaryBleRequestMethod::PerformAndWait(): mtu size too small.");
	}

	LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : read file to segment map." << directoryPath + string("/") + fileName;

	/* 讀檔並輸入map中 */
	fstream file(directoryPath + string("/") + fileName, ios::binary | ios::in);
	//fstream file(directoryPath + string("/Hbivu3xf.mr"), ios::binary | ios::in);

	if (!file.is_open()) {
		LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : unable to open " << directoryPath + string("/") + fileName;
	}

	FileSegmentMap fileSegmentMap(binarySegmentSize);
	fileSegmentMap.ReadFile(&file);
	fileSegmentMap.fileName = fileName;
	
	if (file.is_open())
		file.close();

	// 接下來寫一個builder可以把file segment包成message丟給bluetooth phone，丟完以後再做後續動作如finish command、retransfer等等

	vector<MeteoFileSegmentBluetoothMessage*> fileSegmentMessages;

	for (int i = 0; i < fileSegmentMap.segmentAmount; i++) {
		char* tempFileSegment = nullptr;
		int tempFileSegmentSize = 0;
		tempFileSegmentSize = fileSegmentMap.GetFileSegment(i, &tempFileSegment);

		LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : split file to message [" << i << "] with size " << tempFileSegmentSize;

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
		LOG(LogLevel::Error) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : file segment resolve to no file segment.";
		throw BleRequestException(BleResponseCode::TransferToFileSegmentsFailed);
	}

	// #-- 2 丟檔

	/* 這邊要做丟檔案，每丟一個檔案以後要不停檢查是否接到Ack，才能再丟下一個檔案 */

	LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : start sending file.";

	int tempSendFileSegmentNumber = 0;
	double transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();

	for (int i = 0; i < fileSegmentMessages.size(); i++) {


		if (thisGetBinaryBleRequestHandler->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* 檢查Scene是否還存在，不存在就直接跳出 */
		if (thisGetBinaryBleRequestHandler->isCallbackByScene && !SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequestHandler->callbackScene))
		{
			throw BleRequestException(BleResponseCode::Gone);
		}

		bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[i]);

		/* 每丟一個片段睡10ms */
		this_thread::sleep_for(std::chrono::milliseconds(10));

	}

	while (thisGetBinaryBleRequestHandler->getSectionElapsedSeconds() - transferTempFileSegmentElapsedSeconds > fileSegmentAckTimeout) {
		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisGetBinaryBleRequestHandler->rawMessageMutex);
		while (thisGetBinaryBleRequestHandler->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisGetBinaryBleRequestHandler->inputRawMessages.back();

			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetCommand() == ackTransferCommand &&
				dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetFileName() == fileSegmentMap.fileName) {

				transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
			}
			}
			thisGetBinaryBleRequestHandler->inputRawMessages.pop_back();
			delete message;
		}
	}

	// #-- 3 確認

	LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : send finish message.";
	MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
	json messageContext;
	messageContext["FileName"] = fileSegmentMap.fileName;
	checkFinishMessage->SetContextInJson(messageContext);
	checkFinishMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
	bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

	// 等待對方整理所有需要重傳的file segment
	this_thread::sleep_for(std::chrono::milliseconds(500));

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

		
		/* 這段寫得很長，功能是檢查有沒有全都收到，有的話就離開，沒有就重傳，重傳完要再檢查一次有沒有收到 */
		unique_lock<mutex> uLock(thisGetBinaryBleRequestHandler->rawMessageMutex);
		while (thisGetBinaryBleRequestHandler->inputRawMessages.size() > 0) {

			MeteoBluetoothMessage* message = thisGetBinaryBleRequestHandler->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackFinishCommand) {
				LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : get ack finish.";

				/* 檢查Scene是否還存在，存在才能執行 */
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["Status"].get<int>() == 0) {
					LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : transfer success.";

					for (int i = 0; i < thisGetBinaryBleRequestHandler->inputRawMessages.size(); i++) {
						delete thisGetBinaryBleRequestHandler->inputRawMessages[i];
						thisGetBinaryBleRequestHandler->inputRawMessages.clear();
					}

					return 0;
				}

				LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : transfer not success.";

			}
			else if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == requestRetransferCommand) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["FileName"].get<string>() == fileSegmentMap.fileName) {
				int retransferOrder = dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["Order"].get<int>();

				LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : get retransfer order [" << retransferOrder << "].";
				retransferOrders.push_back(retransferOrder);

			}
			}
			}

			thisGetBinaryBleRequestHandler->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		if (tempSendRetransferOrderIndex < retransferOrders.size() - 1) {

			LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : retransfer segment [" << retransferOrders[tempSendRetransferOrderIndex] << "].";

			tempSendRetransferOrderIndex++;
			transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
			bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);

			this_thread::sleep_for(std::chrono::milliseconds(10));

			/* 從這個時間點開始計時，超過時間就timeout */
			thisGetBinaryBleRequestHandler->writeTimePoint();
		}
		else {
			LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : retransfer over.";

			checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
			json messageContext;
			messageContext["FileName"] = fileSegmentMap.fileName;
			checkFinishMessage->SetContextInJson(messageContext);
			checkFinishMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

			bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

			// 等待對方整理所有需要重傳的file segment
			this_thread::sleep_for(std::chrono::milliseconds(500));
			transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();

		}

	}

	// 結束
	LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::PerformAndWait() : handler over.";

	return 0;

}

int BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::Fail(BleRequest* thisRequest)
{
	LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::Fail() : fail to handle get file request.";

	//if (fileName == "")
	//	return 0;

	BlockingGetBinaryBleRequestHandler* thisGetBinaryBleRequestHandler = dynamic_cast<BlockingGetBinaryBleRequestHandler*>(thisRequest);
	if ((thisGetBinaryBleRequestHandler->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequestHandler->callbackScene)) ||
		!thisGetBinaryBleRequestHandler->isCallbackByScene)
		onFail.TriggerThenClear(fileName);

	return 0;
}

int BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::Success(BleRequest* thisRequest)
{
	LOG(LogLevel::Debug) << "BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::Success() : handle get file request success.";

	//if (fileName == "")
	//	return 0;

	BlockingGetBinaryBleRequestHandler* thisGetBinaryBleRequestHandler = dynamic_cast<BlockingGetBinaryBleRequestHandler*>(thisRequest);
	if ((thisGetBinaryBleRequestHandler->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequestHandler->callbackScene)) ||
		!thisGetBinaryBleRequestHandler->isCallbackByScene)
		onSuccess.TriggerThenClear(fileName);
	return 0;
}

BleRequestMethodType BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::GetMethodType()
{
	return BleRequestMethodType::GetBinaryHandler;
}

int BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name)
{
	onFail.Add(callableObject, callback, name);
	return 0;
}

int BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::AddOnFail(ActionList<int, string>* callback)
{
	onFail.Add(callback);
	return 0;
}

int BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	onSuccess.Add(callableObject, callback, name);
	return 0;
}

int BlockingGetBinaryBleRequestHandler::BlockingGetBinaryBleRequestHandlerMethod::AddOnSuccess(ActionList<int, string>* callback)
{
	onSuccess.Add(callback);
	return 0;
}
