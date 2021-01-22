#include "BleRequest.h"

#include "BleAccess.h"
#include "../../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Output/Bluetooths/MeteoFileSegmentBluetoothMessage.h"
#include <exception>
#include <fstream>
#include "../../../Util/DataStructure/FileSegmentMap.h"
#include "../../../Util/StringSplitter.h"
#include "../../../Framework/Scenes/SceneMaster.h"


using namespace Games::IO::Communications;
using namespace Games::Output::Bluetooths;
using namespace std;
using namespace Util::DataStructure;
using namespace Util;
using namespace Framework::Scenes;



int BleRequest::Perform(CommunicationComponent * cComponent)
{
	communicationComponent = cComponent;

	BleAccess* bleAccess = dynamic_cast<BleAccess*>(communicationComponent);
	// 讓ble access把raw message丟進來，好讓我們檢查有沒有ack或return
	bleAccess->RegisterBleRequest(this);

	requestStartTime = system_clock::now();

	// preform 丟資訊出去然後等回覆

	requestMethod->PerformAndWait(this);

	// 執行完畢以後就不讓ble access把raw message丟進來
	bleAccess->UnregisterBleRequest(this);

	// 如果有出現錯誤，會丟exception，就不會執行on success。這邊要注意request在執行完以後不能直接刪掉，要確定所有task都跑完才能刪
	communicationComponent->GetScheduler()->AddTask([=]() {
		/* 檢查Scene是否還存在，存在才能執行 */
		if(SceneMaster::GetInstance().CheckScene(callbackScene))
			onSuccess.TriggerThenClear();
		return 0;
	});

	return 0;
}

int BleRequest::PushInputRawMessage(MeteoBluetoothMessage * rawMessage)
{
	bool isAcceptMessage = false;

	/* 如果是file segment的話，只有get file request要收。其他狀況就全都收 */
	if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(rawMessage)) {
		if (dynamic_cast<GetBinaryBleRequestMethod*>(requestMethod)) {
			isAcceptMessage = true;
		}
	}
	else {
		isAcceptMessage = true;
	}

	if (isAcceptMessage) {

		unique_lock<mutex> uLock(rawMessageMutex, defer_lock);
		if (uLock.try_lock()) {
			inputRawMessages.push_front(rawMessage);
			uLock.unlock();
		}
		else {
			unique_lock<mutex> uBufferLock(rawMessageBufferMutex);
			inputRawMessagesBuffer.push_front(rawMessage);
		}
	}
	else {
		/* 因為這邊的raw message全都是複製過來的，所以不用的話要delete調 */
		delete rawMessage;
		rawMessage = nullptr;
	}

	return 0;
}


BleRequest::PostTextBleRequestMethod::PostTextBleRequestMethod(MeteoBluetoothMessage* pMessage) : postMessage(pMessage)
{
	isNeedCheckAck = false;
}

BleRequest::PostTextBleRequestMethod::PostTextBleRequestMethod(MeteoBluetoothMessage* pMessage, MeteoCommand aCommand) : postMessage(pMessage)
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
			unique_lock<mutex> uLock(thisRequest->rawMessageMutex);
			for (int i = 0; i < thisRequest->inputRawMessages.size(); i++) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])) {
					if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == ackCommand) {

						/* 檢查Scene是否還存在，存在才能執行 */
						if (SceneMaster::GetInstance().CheckScene(thisRequest->callbackScene))
							onAck.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetContextInJson());

						return 0;

					}
				}
				delete thisRequest->inputRawMessages[i];
			}
			thisRequest->inputRawMessages.clear();

			uLock.unlock();

			this_thread::sleep_for(std::chrono::milliseconds(100));

			/* 舊的程式，寫得不好
			mutex* inputRawCommandMutex = bleAccess->GetRawCommandMutex();
			unique_lock<mutex> uLock(*inputRawCommandMutex);

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
	}


	return 0;
}

int BleRequest::PostTextBleRequestMethod::AddOnAck(BleRequest* thisRequest, MtoObject * callableObject, function<int(json)> callback, string name)
{
	/* 不是scene就不能用request的callback */
	if (dynamic_cast<Scene*>(callableObject) == nullptr)
		return -1;

	/* 紀錄callback scene */
	if (thisRequest->callbackScene == nullptr)
		thisRequest->callbackScene = dynamic_cast<Scene*>(callableObject);

	/* 不允許由不同scene註冊，會當作error */
	if (thisRequest->callbackScene != dynamic_cast<Scene*>(callableObject))
		return -1;

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
	double tempElapsedTime = thisRequest->getElapsedSeconds();
	bool isFinished = false;
	bool isRetransferred = false;

	while (1) {

		if (thisRequest->getElapsedSeconds() - tempElapsedTime > thisRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisRequest->rawMessageMutex);
		for (int i = 0; i < thisRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == ackFinishCommand) {


					/* 檢查Scene是否還存在，存在才能執行 */
					if (SceneMaster::GetInstance().CheckScene(thisRequest->callbackScene))
						onFinish.TriggerThenClear();
					isFinished = true;
					return 0;

				}
				else if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == requestRetransferCommand) {
					if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetContextInJson()["FileName"].get<string>() == fileName) {
						int retransferOrder = dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetContextInJson()["Order"].get<int>();

						// TODO 重傳這個file segment
						retransferOrders.push_back(retransferOrder);
					}
				}
			}
			delete thisRequest->inputRawMessages[i];
		}
		thisRequest->inputRawMessages.clear();

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

	

	return 0;
}

int BleRequest::PostBinaryBleRequestMethod::AddOnFinish(MtoObject * callableObject, function<int()> callback, string name)
{
	onFinish.Add(callableObject, callback, name);
	return 0;
}

BleRequest::GetTextBleRequestMethod::GetTextBleRequestMethod(MeteoBluetoothMessage * gMessage, MeteoCommand rCommand) : getMessage(gMessage)
{
	returnCommand = rCommand;
}

int BleRequest::GetTextBleRequestMethod::PerformAndWait(BleRequest * thisRequest)
{
	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<BleAccess*>(thisRequest->communicationComponent)->GetPeripheral());

	MeteoContextBluetoothMessage* outputMessage = nullptr;

	bluetoothPhone->PushOutputMessage(getMessage);

	while (1) {

		if (thisRequest->getElapsedSeconds() > thisRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* 確認收到的command是不是return command */
		unique_lock<mutex> uLock(thisRequest->rawMessageMutex);
		for (int i = 0; i < thisRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == returnCommand) {

					// 確認Scene還存不存在，如果不存在就不執行callback，避免該scene已經被刪掉的情況
					/* 檢查Scene是否還存在，存在才能執行 */
					if (SceneMaster::GetInstance().CheckScene(thisRequest->callbackScene))
						onReturn.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetContextInJson());

					return 0;

				}
			}
			delete thisRequest->inputRawMessages[i];
		}
		thisRequest->inputRawMessages.clear();

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));


		
		/* 這段成是寫得不好，不用了
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

BleRequest::GetBinaryBleRequestMethod::GetBinaryBleRequestMethod(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
{
	vector<string> filePathHierachy = StringSplitter::Split(fPath, '/', '\\');
	fileName = filePathHierachy[filePathHierachy.size() - 1];
	filePathHierachy.pop_back();
	directoryPath = StringSplitter::Combine(filePathHierachy, '/');

	getMessage = gMessage;
	ackGetCommand = aGetCommand;
	transferCommand = tCommand;
	finishCommand = fCommand;
	requestRetransferCommand = rRetransferCommand;
	ackFinishCommand = aFinishCommand;

}

int BleRequest::GetBinaryBleRequestMethod::PerformAndWait(BleRequest * thisRequest)
{
	/* 抓目前的藍芽mtu */
	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<BleAccess*>(thisRequest->communicationComponent)->GetPeripheral());


	bluetoothPhone->PushOutputMessage(getMessage);

	while (1) {

		if (thisRequest->getElapsedSeconds() > thisRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* 這段寫得很長，功能就只是把收到的ack丟給request而已 */
		unique_lock<mutex> uLock(thisRequest->rawMessageMutex);
		for (int i = 0; i < thisRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == ackGetCommand) {

					/* 檢查Scene是否還存在，存在才能執行 */
					if (SceneMaster::GetInstance().CheckScene(thisRequest->callbackScene))
						onAck.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetContextInJson());

					return 0;

				}
			}
			delete thisRequest->inputRawMessages[i];
		}
		thisRequest->inputRawMessages.clear();

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

		if (thisRequest->getElapsedSeconds() > thisRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* 確認收到的command是不是return command */
		unique_lock<mutex> uLock(thisRequest->rawMessageMutex);
		for (int i = 0; i < thisRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(thisRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == transferCommand) {

					MeteoFileSegmentBluetoothMessage* fileSegmentMessage = dynamic_cast<MeteoFileSegmentBluetoothMessage*>(thisRequest->inputRawMessages[i]);
					
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
			delete thisRequest->inputRawMessages[i];
		}
		thisRequest->inputRawMessages.clear();

		uLock.unlock();
	}

	// file segment map write file

	fstream file(directoryPath + string("/") + fileName, ios::out | ios::binary);

	bleBinaryRequestFileSegmentMap.WriteFile(&file);

	/* 告知已收到檔案 */
	while (!isAckedTransferFinished) {

		/* 確認收到的command是不是return command */
		unique_lock<mutex> uLock(thisRequest->rawMessageMutex);
		for (int i = 0; i < thisRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == finishCommand) {

					MeteoContextBluetoothMessage* ackFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
					json messageContext;
					messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
					ackFinishMessage->SetContextInJson(messageContext);
					bleAccess->GetBluetoothPhone()->PushOutputMessage(ackFinishMessage);

					isAckedTransferFinished = true;
				}
			}
			delete thisRequest->inputRawMessages[i];
		}
		thisRequest->inputRawMessages.clear();

		uLock.unlock();

	}

	/* 檢查Scene是否還存在，存在才能執行 */
	if (SceneMaster::GetInstance().CheckScene(thisRequest->callbackScene))
		onFinish.TriggerThenClear();



	return 0;
}

BleRequest::BleBinaryRequestFileSegmentMap::~BleBinaryRequestFileSegmentMap()
{

	for (map<int, pair<char*, int>>::const_iterator it = fileSegmentMap.begin(); it != fileSegmentMap.end(); ++it)
	{
		delete[] it->second.first;
	}
	fileSegmentMap.clear();
}

bool BleRequest::BleBinaryRequestFileSegmentMap::CheckFullFilled()
{
	if (segmentAmount == fileSegmentMap.size())
		return true;
	return false;
}

int BleRequest::BleBinaryRequestFileSegmentMap::WriteFile(fstream * fStream)
{
	for (map<int, pair<char*, int>>::const_iterator it = fileSegmentMap.begin(); it != fileSegmentMap.end(); ++it)
	{
		fStream->write(it->second.first, it->second.second * sizeof(char*));

	}

	return 0;
}

int BleRequest::BleBinaryRequestFileSegmentMap::ReadFile(fstream * fStream)
{
	/* 計算檔案大小 */
	fStream->seekp(0, ios_base::beg);
	streampos fileSize = fStream->tellg();
	fStream->seekg(0, std::ios::end);
	fileSize = fStream->tellg() - fileSize;

	/* 計算segment數量 */
	segmentAmount = fileSize / segmentSize + fileSize % segmentSize > 0 ? 1 : 0;

	/* 開始讀黨 */
	fStream->seekp(0, ios_base::beg);
	for (int i = 0; i < segmentAmount; i++) {

		char* buffer = nullptr;
		int bufferSize = 0;

		if (i == segmentAmount - 1 && fileSize % segmentSize != 0)
			bufferSize = fileSize % segmentSize;
		else 
			bufferSize = segmentSize;

		buffer = new char[bufferSize];
		fStream->read(buffer, bufferSize);
		fileSegmentMap[i] = pair<char*, int>(buffer, bufferSize);

	}

	return 0;
}
