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
	// ��ble access��raw message��i�ӡA�n���ڭ��ˬd���S��ack��return
	bleAccess->RegisterBleRequest(this);

	requestStartTime = system_clock::now();

	// preform ���T�X�h�M�ᵥ�^��

	requestMethod->PerformAndWait(this);

	// ���槹���H��N����ble access��raw message��i��
	bleAccess->UnregisterBleRequest(this);

	// �p�G���X�{���~�A�|��exception�A�N���|����on success�C�o��n�`�Nrequest�b���槹�H�ᤣ�ઽ���R���A�n�T�w�Ҧ�task���]���~��R
	communicationComponent->GetScheduler()->AddTask([=]() {
		/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
		if(SceneMaster::GetInstance().CheckScene(callbackScene))
			onSuccess.TriggerThenClear();
		return 0;
	});

	return 0;
}

int BleRequest::PushInputRawMessage(MeteoBluetoothMessage * rawMessage)
{
	bool isAcceptMessage = false;

	/* �p�G�Ofile segment���ܡA�u��get file request�n���C��L���p�N������ */
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
		/* �]���o�䪺raw message�����O�ƻs�L�Ӫ��A�ҥH���Ϊ��ܭndelete�� */
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

			/* �o�q�g�o�ܪ��A�\��N�u�O�⦬�쪺ack�ᵹrequest�Ӥw */
			unique_lock<mutex> uLock(thisRequest->rawMessageMutex);
			for (int i = 0; i < thisRequest->inputRawMessages.size(); i++) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])) {
					if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == ackCommand) {

						/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
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

			/* �ª��{���A�g�o���n
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
	/* ���Oscene�N�����request��callback */
	if (dynamic_cast<Scene*>(callableObject) == nullptr)
		return -1;

	/* ����callback scene */
	if (thisRequest->callbackScene == nullptr)
		thisRequest->callbackScene = dynamic_cast<Scene*>(callableObject);

	/* �����\�Ѥ��Pscene���U�A�|��@error */
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
	/* ��ثe���Ū�mtu */
	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisRequest->communicationComponent);
	int mtu = bleAccess->GetMtu();

	int binarySegmentSize = mtu - 28;
	if (binarySegmentSize <= 0) {
		LOG(LogLevel::Error) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : mtu size " << mtu << " too small.";
		throw logic_error("BleRequest::PostBinaryBleRequestMethod::PerformAndWait(): mtu size too small.");
	}

	/* Ū�ɨÿ�Jmap�� */
	fstream file(filePath, ios::binary);

	FileSegmentMap bleBinaryRequestFileSegmentMap(binarySegmentSize);
	bleBinaryRequestFileSegmentMap.ReadFile(&file);
	vector<string> splitPath = StringSplitter::Split(filePath, '/', '\\');
	string fileName = splitPath[splitPath.size() - 1];
	bleBinaryRequestFileSegmentMap.fileName = fileName;

	// ���U�Ӽg�@��builder�i�H��file segment�]��message�ᵹbluetooth phone�A�᧹�H��A������ʧ@�pfinish command�Bretransfer����

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

	/* �����ɮץ�����X�h�A�A�e�X�ˬd�T���A�й���ˬd�O�_�������� */
	while (!bleAccess->GetBluetoothPhone()->CheckFileSegmentMessageOutputClear()) {
		// TODO: �p�G���Ӥ[�A�N��exception
	}

	MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
	json messageContext;
	messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
	checkFinishMessage->SetContextInJson(messageContext);
	bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

	/* �q�o��}�l�p�ɡA�p�G�L�Ӥ[�Ntimeout�C�p�G�����ǴN�n����@��check finish */
	double tempElapsedTime = thisRequest->getElapsedSeconds();
	bool isFinished = false;
	bool isRetransferred = false;

	while (1) {

		if (thisRequest->getElapsedSeconds() - tempElapsedTime > thisRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* �o�q�g�o�ܪ��A�\��N�u�O�⦬�쪺ack�ᵹrequest�Ӥw */
		unique_lock<mutex> uLock(thisRequest->rawMessageMutex);
		for (int i = 0; i < thisRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == ackFinishCommand) {


					/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
					if (SceneMaster::GetInstance().CheckScene(thisRequest->callbackScene))
						onFinish.TriggerThenClear();
					isFinished = true;
					return 0;

				}
				else if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == requestRetransferCommand) {
					if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetContextInJson()["FileName"].get<string>() == fileName) {
						int retransferOrder = dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetContextInJson()["Order"].get<int>();

						// TODO ���ǳo��file segment
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
				// TODO: �p�G���Ӥ[�A�N��exception
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

		// bleAccess->GetInputRawCommand().clear();	���ble access�ۤv�M
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

		/* �T�{���쪺command�O���Oreturn command */
		unique_lock<mutex> uLock(thisRequest->rawMessageMutex);
		for (int i = 0; i < thisRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == returnCommand) {

					// �T�{Scene�٦s���s�b�A�p�G���s�b�N������callback�A�קK��scene�w�g�Q�R�������p
					/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
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


		
		/* �o�q���O�g�o���n�A���ΤF
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
	/* ��ثe���Ū�mtu */
	BleAccess* bleAccess = dynamic_cast<BleAccess*>(thisRequest->communicationComponent);
	BluetoothPhone* bluetoothPhone = dynamic_cast<BluetoothPhone*>(dynamic_cast<BleAccess*>(thisRequest->communicationComponent)->GetPeripheral());


	bluetoothPhone->PushOutputMessage(getMessage);

	while (1) {

		if (thisRequest->getElapsedSeconds() > thisRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* �o�q�g�o�ܪ��A�\��N�u�O�⦬�쪺ack�ᵹrequest�Ӥw */
		unique_lock<mutex> uLock(thisRequest->rawMessageMutex);
		for (int i = 0; i < thisRequest->inputRawMessages.size(); i++) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(thisRequest->inputRawMessages[i])->GetCommand() == ackGetCommand) {

					/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
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


	/* �o��}�l����� */
	while (!isTransferFinished) {

		if (thisRequest->getElapsedSeconds() > thisRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		/* �T�{���쪺command�O���Oreturn command */
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

	/* �i���w�����ɮ� */
	while (!isAckedTransferFinished) {

		/* �T�{���쪺command�O���Oreturn command */
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

	/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
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
	/* �p���ɮפj�p */
	fStream->seekp(0, ios_base::beg);
	streampos fileSize = fStream->tellg();
	fStream->seekg(0, std::ios::end);
	fileSize = fStream->tellg() - fileSize;

	/* �p��segment�ƶq */
	segmentAmount = fileSize / segmentSize + fileSize % segmentSize > 0 ? 1 : 0;

	/* �}�lŪ�� */
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
