#include "BleRequest.h"

#include "BleAccess.h"
#include "../../Input/Commands/MeteoTextBluetoothCommand.h"
#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Output/Bluetooths/MeteoFileSegmentBluetoothMessage.h"
#include <exception>
#include <fstream>
#include "../../../Util/DataStructure/FileSegmentMap.h"
#include "../../../Util/StringSplitter.h"


using namespace Games::IO::Communications;
using namespace Games::Input::Commands;
using namespace Games::Output::Bluetooths;
using namespace std;
using namespace Util::DataStructure;
using namespace Util;



int BleRequest::Perform(CommunicationComponent * cComponent)
{
	communicationComponent = cComponent;

	requestStartTime = system_clock::now();

	// preform 丟資訊出去然後等回覆

	requestMethod->PerformAndWait(this);

	// 如果有出現錯誤，會丟exception，就不會執行on success

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

	MeteoContextBluetoothMessage* outputMessage = nullptr;
	// TODO: command to message
	bluetoothPhone->PushOutputMessage(outputMessage);

	if (isNeedCheckAck) {

		while (1) {

			if (thisRequest->getElapsedSeconds() > thisRequest->timeout) {
				throw BleRequestException(BleResponseCode::RequestTimeout);
			}

			/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
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

BleRequest::PostBinaryBleRequestMethod::PostBinaryBleRequestMethod(string fPath, MeteoCommand tCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
{
	filePath = fPath;
	transferCommand = tCommand;
	finishCommand = fCommand;
	requestRetransferCommand = rRetransferCommand;
	ackFinishCommand = aFinishCommand;



}

int BleRequest::PostBinaryBleRequestMethod::PerformAndWait(BleRequest * thisRequest)
{
	/* 抓目前的藍芽mtu */
	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisRequest->communicationComponent);
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
	vector<string> splitPath = StringSplitter::Split(filePath, '/', '\\');
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

		fileSegmentMessages.push_back(fileSegmentMessage);

		bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessage);
	}

	/* 等待檔案全都丟出去，再送出檢查訊息，請對方檢查是否全部收到 */
	while (!bleAccess->GetBluetoothPhone()->CheckFileSegmentMessageOutputClear()) {
		// TODO: 如果等太久，就丟exception
	}

	MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
	checkFinishMessage->GetContext()["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
	bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

	/* 從這邊開始計時，如果過太久就timeout。如果有重傳就要重丟一次check finish */
	double tempElapsedTime = thisRequest->getElapsedSeconds();
	bool isFinished = false;
	bool isRetransferred = false;

	while (1) {

		if (thisRequest->getElapsedSeconds() - tempElapsedTime > thisRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(*bleAccess->GetRawCommandMutex());
		for (int i = 0; i < bleAccess->GetInputRawCommand()->size(); i++) {
			if (dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()->at(i))) {
				if (dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()->at(i))->GetCommand() == ackFinishCommand) {

					onFinish.TriggerThenClear();
					bleAccess->GetInputRawCommand()->erase(bleAccess->GetInputRawCommand()->begin() + i);
					i--;
					isFinished = true;
					break;

				}
				else if (dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()->at(i))->GetCommand() == requestRetransferCommand) {
					if (dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()->at(i))->GetContextInJson()["FileName"].get<string>() == fileName) {
						int retransferOrder = dynamic_cast<MeteoTextBluetoothCommand*>(bleAccess->GetInputRawCommand()->at(i))->GetContextInJson()["Order"].get<int>();

						bleAccess->GetInputRawCommand()->erase(bleAccess->GetInputRawCommand()->begin() + i);
						i--;
					}
				}
			}
		}
		uLock.unlock();

		if (isFinished)
			break;

		if (isRetransferred) {
			while (!bleAccess->GetBluetoothPhone()->CheckFileSegmentMessageOutputClear()) {
				// TODO: 如果等太久，就丟exception
			}
			MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
			checkFinishMessage->GetContext()["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
			bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);
		}

		thread sleep(0.01);

		// bleAccess->GetInputRawCommand().clear();	改由ble access自己清
	}

	

	return 0;
}

int BleRequest::PostBinaryBleRequestMethod::AddOnFinish(MtoObject * callableObject, function<int()> callback, string name)
{
	onFinish.Add(callableObject, callback, name);
	return 0;
}

BleRequest::GetTextBleRequestMethod::GetTextBleRequestMethod(MeteoBluetoothCommand * gMessage, MeteoCommand rCommand) : getMessage(gMessage)
{
	returnCommand = rCommand;
}

int BleRequest::GetTextBleRequestMethod::PerformAndWait(BleRequest * thisRequest)
{
	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<BleAccess*>(thisRequest->communicationComponent)->GetPeripheral());

	MeteoContextBluetoothMessage* outputMessage = nullptr;

	bluetoothPhone->PushOutputMessage(outputMessage);

	if (isNeedCheckAck) {

		while (1) {

			if (thisRequest->getElapsedSeconds() > thisRequest->timeout) {
				throw BleRequestException(BleResponseCode::RequestTimeout);
			}

			/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
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
