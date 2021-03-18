#include "PostBinaryBleRequest.h"


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



PostBinaryBleRequest::PostBinaryBleRequestMethod::PostBinaryBleRequestMethod(string fPath,
	MeteoBluetoothMessage* gMessage,
	MeteoCommand ackGetCommand, 
	MeteoCommand tCommand, 
	MeteoCommand fCommand, 
	MeteoCommand rRetransferCommand, 
	MeteoCommand aFinishCommand)
{
	filePath = fPath;


	transferCommand = tCommand;
	finishCommand = fCommand;
	requestRetransferCommand = rRetransferCommand;
	ackFinishCommand = aFinishCommand;



}

int PostBinaryBleRequest::PostBinaryBleRequestMethod::PerformAndWait(BleRequest * thisRequest)
{
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

		bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessage);
	}

	/* 等待檔案全都丟出去，再送出檢查訊息，請對方檢查是否全部收到 */
	while (!bleAccess->GetBluetoothPhone()->CheckFileSegmentMessageOutputClear()) {
		if (thisPostBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}
		// TODO: 如果等太久，就丟exception
	}

	MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
	json messageContext;
	messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
	checkFinishMessage->SetContextInJson(messageContext);
	bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);


	bool isFinished = false;
	bool isRetransferred = false;

	/* 從這個時間點開始計時，超過時間就timeout */
	thisPostBinaryRequest->writeTimePoint();

	while (1) {

		if (thisPostBinaryRequest->getSectionElapsedSeconds() > thisPostBinaryRequest->timeout) {
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
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackFinishCommand) {


					/* 檢查Scene是否還存在，存在才能執行 */
					if ((thisPostBinaryRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisPostBinaryRequest->callbackScene)) ||
						!thisPostBinaryRequest->isCallbackByScene ||
						onFinish.GetSize() == 0)
						onFinish.TriggerThenClear();
					else {
						throw BleRequestException(BleResponseCode::Gone);
					}


					isFinished = true;
					return 0;

				}
				else if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == requestRetransferCommand) {
					if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["FileName"].get<string>() == fileName) {
						int retransferOrder = dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["Order"].get<int>();

						// TODO 重傳這個file segment
						retransferOrders.push_back(retransferOrder);

					}
				}
			}
			thisPostBinaryRequest->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		if (retransferOrders.size() > 0) {

			for (int i = 0; i < retransferOrders.size(); i++) {

				char* tempFileSegment = nullptr;
				int tempFileSegmentSize = 0;
				tempFileSegmentSize = bleBinaryRequestFileSegmentMap.GetFileSegment(retransferOrders[i], &tempFileSegment);

				MeteoFileSegmentBluetoothMessage* fileSegmentMessage = new MeteoFileSegmentBluetoothMessage(
					transferCommand,
					tempFileSegment,
					tempFileSegmentSize,
					bleBinaryRequestFileSegmentMap.fileName,
					retransferOrders[i],
					bleBinaryRequestFileSegmentMap.segmentAmount);

				delete[] tempFileSegment;

				fileSegmentMessages.push_back(fileSegmentMessage);

				bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessage);

			}

			retransferOrders.clear();



			while (!bleAccess->GetBluetoothPhone()->CheckFileSegmentMessageOutputClear()) {
				if (thisPostBinaryRequest->exitRequested) {
					throw BleRequestException(BleResponseCode::ExitRequested);
				}

				// TODO: 如果等太久，就丟exception
			}


			/* 從這個時間點開始計時，超過時間就timeout */
			thisPostBinaryRequest->writeTimePoint();

			MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
			json messageContext;
			messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
			checkFinishMessage->SetContextInJson(messageContext);
			bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

		}

		if (isFinished)
			break;

		this_thread::sleep_for(std::chrono::milliseconds(100));

	}

	/* 檢查Scene是否還存在，存在才能執行 */
	if ((thisPostBinaryRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisPostBinaryRequest->callbackScene)) ||
		!thisPostBinaryRequest->isCallbackByScene ||
		onFinish.GetSize() == 0)
		onFinish.TriggerThenClear();
	else
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

PostBinaryBleRequest::PostBinaryBleRequest(string fPath, MeteoBluetoothMessage * pMessage, MeteoCommand ackPostCommand, MeteoCommand tCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand) : RegisterType("PostBinaryBleRequest")
{
	PostBinaryBleRequestMethod* method = new PostBinaryBleRequestMethod(
		fPath,
		pMessage,
		ackPostCommand,
		tCommand,
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
