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

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisPostBinaryRequest->rawMessageMutex);
		for (int i = 0; i < thisPostBinaryRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisPostBinaryRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisPostBinaryRequest->inputRawMessages[i])->GetCommand() == ackPostCommand) {

					/* 檢查Scene是否還存在，存在才能執行 */
					if (SceneMaster::GetInstance().CheckScene(thisPostBinaryRequest->callbackScene))
						onAck.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(thisPostBinaryRequest->inputRawMessages[i])->GetContextInJson());
					else {
						// fail
						BleRequestException except(BleResponseCode::Gone);
						thisPostBinaryRequest->Fail(except);

					}

					isAckReceived = true;

				}
			}
			delete thisPostBinaryRequest->inputRawMessages[i];
		}
		thisPostBinaryRequest->inputRawMessages.clear();

		uLock.unlock();
	}

	int mtu = bleAccess->GetMtu();

	int binarySegmentSize = mtu - 28;
	if (binarySegmentSize <= 0) {
		LOG(LogLevel::Error) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : mtu size " << mtu << " too small.";
		throw logic_error("BleRequest::PostBinaryBleRequestMethod::PerformAndWait(): mtu size too small.");
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
		// TODO: 如果等太久，就丟exception
	}

	MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
	json messageContext;
	messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
	checkFinishMessage->SetContextInJson(messageContext);
	bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

	/* 從這邊開始計時，如果過太久就timeout。如果有重傳就要重丟一次check finish */
	double tempElapsedTime = thisPostBinaryRequest->getElapsedSeconds();
	bool isFinished = false;
	bool isRetransferred = false;

	while (1) {

		if (thisPostBinaryRequest->getElapsedSeconds() - tempElapsedTime > thisPostBinaryRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisPostBinaryRequest->rawMessageMutex);
		for (int i = 0; i < thisPostBinaryRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisPostBinaryRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisPostBinaryRequest->inputRawMessages[i])->GetCommand() == ackFinishCommand) {


					/* 檢查Scene是否還存在，存在才能執行 */
					if (SceneMaster::GetInstance().CheckScene(thisPostBinaryRequest->callbackScene))
						onFinish.TriggerThenClear();
					isFinished = true;
					return 0;

				}
				else if (dynamic_cast<MeteoContextBluetoothMessage*>(thisPostBinaryRequest->inputRawMessages[i])->GetCommand() == requestRetransferCommand) {
					if (dynamic_cast<MeteoContextBluetoothMessage*>(thisPostBinaryRequest->inputRawMessages[i])->GetContextInJson()["FileName"].get<string>() == fileName) {
						int retransferOrder = dynamic_cast<MeteoContextBluetoothMessage*>(thisPostBinaryRequest->inputRawMessages[i])->GetContextInJson()["Order"].get<int>();

						// TODO 重傳這個file segment
						retransferOrders.push_back(retransferOrder);
					}
				}
			}
			delete thisPostBinaryRequest->inputRawMessages[i];
		}
		thisPostBinaryRequest->inputRawMessages.clear();

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
				// TODO: 如果等太久，就丟exception
			}

			MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
			json messageContext;
			messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
			checkFinishMessage->SetContextInJson(messageContext);
			bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

		}



		if (isFinished)
			break;


		this_thread::sleep_for(std::chrono::milliseconds(100));

		// bleAccess->GetInputRawCommand().clear();	改由ble access自己清
	}

	/* 檢查Scene是否還存在，存在才能執行 */
	if (SceneMaster::GetInstance().CheckScene(thisPostBinaryRequest->callbackScene))
		onFinish.TriggerThenClear();

	return 0;
}

BleRequestMethodType PostBinaryBleRequest::PostBinaryBleRequestMethod::GetMethodType()
{
	return BleRequestMethodType::PostBinary;
}

int PostBinaryBleRequest::PostBinaryBleRequestMethod::AddOnFinish(MtoObject * callableObject, function<int()> callback, string name)
{
	onFinish.Add(callableObject, callback, name);
	return 0;
}

PostBinaryBleRequest::PostBinaryBleRequest(string fPath, MeteoBluetoothMessage * pMessage, MeteoCommand ackPostCommand, MeteoCommand tCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
{
	LOG(LogLevel::Error) << "PostBinaryBleRequest::PostBinaryBleRequest() : not implemented.";
}

int PostBinaryBleRequest::ChooseCommunicationComponentToPerform()
{

	LOG(LogLevel::Error) << "PostBinaryBleRequest::ChooseCommunicationComponentToPerform() : not implemented.";
	return 0;
}

int PostBinaryBleRequest::AddOnFinish(MtoObject * callableObject, function<int()> callback, string name)
{
	LOG(LogLevel::Error) << "PostBinaryBleRequest::AddOnFinish() : not implemented.";
	return 0;
}

int PostBinaryBleRequest::fail(exception & e)
{
	LOG(LogLevel::Error) << "PostBinaryBleRequest::fail() : not implemented.";
	return 0;
}
