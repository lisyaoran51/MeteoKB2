#include "GetBinaryBleRequest.h"

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

	LOG(LogLevel::Finer) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : start handling request.";


	bluetoothPhone->PushOutputMessage(getMessage);

	bool isAckReceived = false;
	int ackValue = 0;

	while (!isAckReceived) {

		if (thisGetBinaryRequest->getElapsedSeconds() > thisGetBinaryRequest->timeout) {
			LOG(LogLevel::Finer) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : waiting ack time out.";
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

				LOG(LogLevel::Finer) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : get ack message." << dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContext();


				isAckReceived = true;
				ackValue = dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["Status"].get<int>();

			}
			}

			thisGetBinaryRequest->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));

	}

	if (ackValue != 0) {
		LOG(LogLevel::Warning) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : ack value is not 0 [" << ackValue << "].";

		switch (ackValue) {
		case -1:
			throw BleRequestException(BleResponseCode::AppFileNotFound);
			break;
		}
	}

	int mtu = bleAccess->GetMtu();

	int binarySegmentSize = mtu - 28;
	if (binarySegmentSize <= 0) {
		LOG(LogLevel::Error) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : mtu size " << mtu << " too small.";

		throw BleRequestException(BleResponseCode::MtuTooSmall);
	}

	LOG(LogLevel::Finer) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : build file segments.";

	fileSegmentMap = new FileSegmentMap(binarySegmentSize);
	fileSegmentMap->fileName = fileName;
	fileSegmentMap->segmentAmount = -1;	// 預設一個數，之後會再補上真實大小

	/* 從這個時間點開始計時，超過時間就timeout */
	thisGetBinaryRequest->writeTimePoint();

	/* 這邊開始收資料 */
	while (!isTransferFinished) {

		if (thisGetBinaryRequest->getSectionElapsedSeconds() > thisGetBinaryRequest->timeout) {
			LOG(LogLevel::Finer) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : waiting ack time out.";
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

				int amount = fileSegmentMessage->GetAmount();
				if(fileSegmentMap->segmentAmount == -1)
					fileSegmentMap->segmentAmount = amount;

				LOG(LogLevel::Finer) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : file [" << fileName << "] get segment [" << order << "/" << amount << "].";

				if (fileSegmentMap->segmentAmount != amount) {
					LOG(LogLevel::Error) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : local segment amout not fit packet [" << fileSegmentMap->segmentAmount << "/" << amount << "]";
					throw BleRequestException(BleResponseCode::WrongSegmentAmount);
				}

				if (order >= amount) {
					LOG(LogLevel::Error) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : segment order exceed segment counts. [" << order << "/" << fileSegmentMap->segmentAmount << "]";
					throw BleRequestException(BleResponseCode::FileSegmentOrderExceedSize);
				}

				fileSegmentMap->fileSegmentMap[order] = pair<char*, int>(fileSegmentData, size);

				MeteoAckFileSegmentBluetoothMessage* ackFileSegmentMessage = new MeteoAckFileSegmentBluetoothMessage(ackTransferCommand, fileName, order, amount);

				bluetoothPhone->PushOutputMessage(ackFileSegmentMessage);

				/* 從這個時間點開始計時，超過時間就timeout */
				thisGetBinaryRequest->writeTimePoint();

			}
			}
			else if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == finishCommand) {
				
				LOG(LogLevel::Debug) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : get finish command.";

				if (fileSegmentMap->fileSegmentMap.size() == fileSegmentMap->segmentAmount) {
					isTransferFinished = true;
				}
				else {
					LOG(LogLevel::Depricated) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : segments not enough [" << fileSegmentMap->fileSegmentMap.size() << "].";

					map<int, pair<char*, int>>::iterator it;
					for (it = fileSegmentMap->fileSegmentMap.begin(); it != fileSegmentMap->fileSegmentMap.end(); ++it) {
						LOG(LogLevel::Depricated) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : already have segment [" << (*it).first << "].";
					}

					MeteoContextBluetoothMessage* ackFinishferMessage = new MeteoContextBluetoothMessage(ackFinishCommand);

					json messageContext;
					messageContext["FileName"] = fileSegmentMap->fileName;
					messageContext["Status"] = -1;

					ackFinishferMessage->SetContextInJson(messageContext);
					ackFinishferMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
					bleAccess->GetBluetoothPhone()->PushOutputMessage(ackFinishferMessage);
				}

				for (int i = 0; i < fileSegmentMap->segmentAmount; i++) {
					map<int, pair<char*, int>>::iterator iter;
					iter = fileSegmentMap->fileSegmentMap.find(i);
					if (iter == fileSegmentMap->fileSegmentMap.end()) {

						LOG(LogLevel::Debug) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : request rewrite segment [" << i << "].";

						MeteoContextBluetoothMessage* reuqestRetransferMessage = new MeteoContextBluetoothMessage(requestRetransferCommand);

						json messageContext;
						messageContext["FileName"] = fileSegmentMap->fileName;
						messageContext["Order"] = i;

						reuqestRetransferMessage->SetContextInJson(messageContext);
						reuqestRetransferMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
						bleAccess->GetBluetoothPhone()->PushOutputMessage(reuqestRetransferMessage);
					}
				}

				/* 留一點時間給對方緩衝 */
				//if(!isTransferFinished)
				//	this_thread::sleep_for(std::chrono::milliseconds(100));

				/* 從這個時間點開始計時，超過時間就timeout */
				thisGetBinaryRequest->writeTimePoint();
			}}

			thisGetBinaryRequest->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

	}

	LOG(LogLevel::Finer) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : finish get file.";

	MeteoContextBluetoothMessage* ackFinishferMessage = new MeteoContextBluetoothMessage(ackFinishCommand);

	json messageContext;
	messageContext["FileName"] = fileSegmentMap->fileName;
	messageContext["Status"] = 0;

	ackFinishferMessage->SetContextInJson(messageContext);
	ackFinishferMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
	bleAccess->GetBluetoothPhone()->PushOutputMessage(ackFinishferMessage);

	LOG(LogLevel::Finer) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait() : write file to [" << directoryPath + string("/") + fileName << "]";
	// file segment map write file
	fstream file(directoryPath + string("/") + fileName, ios::out | ios::binary);

	fileSegmentMap->WriteFile(&file);


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
	LOG(LogLevel::Debug) << "GetBinaryBleRequest::GetBinaryBleRequestMethod::Success() : get file success.";

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

int GetBinaryBleRequest::GetBinaryBleRequestMethod::AddOnSuccess(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name)
{
	onSuccess.Add(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::AddOnSuccess(ActionList<int, FileSegmentMap*>* callback)
{
	onSuccess.Add(callback);
	return 0;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::AddOnFail(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name)
{
	onFail.Add(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::AddOnFail(ActionList<int, FileSegmentMap*>* callback)
{
	onFail.Add(callback);
	return 0;
}

GetBinaryBleRequest::GetBinaryBleRequest(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand ackGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand) : RegisterType("GetBinaryBleRequest")
{
	GetBinaryBleRequestMethod* method = new GetBinaryBleRequestMethod(fPath,
		gMessage,
		ackGetCommand,
		tCommand,
		aTransferCommand,
		fCommand,
		rRetransferCommand,
		aFinishCommand);

	requestMethod = method;


	// 測試用
	timeout = 20;
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
