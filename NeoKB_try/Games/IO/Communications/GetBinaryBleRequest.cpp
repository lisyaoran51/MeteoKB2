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



GetBinaryBleRequest::GetBinaryBleRequestMethod::GetBinaryBleRequestMethod(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
{
	vector<string> filePathHierachy = StringSplitter::Split(fPath, "/");
	fileName = filePathHierachy[filePathHierachy.size() - 1];
	filePathHierachy.pop_back();
	directoryPath = StringSplitter::Combine(filePathHierachy, "/");

	getMessage = gMessage;
	ackGetCommand = aGetCommand;
	transferCommand = tCommand;
	finishCommand = fCommand;
	requestRetransferCommand = rRetransferCommand;
	ackFinishCommand = aFinishCommand;

}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::PerformAndWait(BleRequest * thisRequest)
{
	GetBinaryBleRequest* thisGetBinaryRequest = dynamic_cast<GetBinaryBleRequest*>(thisRequest);

	/* ��ثe���Ū�mtu */
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

		/* �o�q�g�o�ܪ��A�\��N�u�O�⦬�쪺ack�ᵹrequest�Ӥw */
		unique_lock<mutex> uLock(thisGetBinaryRequest->rawMessageMutex);
		while(thisGetBinaryRequest->inputRawMessages.size() > 0) {

			MeteoBluetoothMessage* message = thisGetBinaryRequest->inputRawMessages.back();
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackGetCommand) {

					/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
					if ((thisGetBinaryRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryRequest->callbackScene)) ||
						!thisGetBinaryRequest->isCallbackByScene)
						onAck.TriggerThenClear(dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson());
					else {
						throw BleRequestException(BleResponseCode::Gone);
					}

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
		// TODO: ���Ӥ��_�s�u�ó����A�����Ӫ�����
	}

	FileSegmentMap bleBinaryRequestFileSegmentMap(binarySegmentSize);
	bleBinaryRequestFileSegmentMap.segmentAmount = 100;	// �w�]�@�ӼơA����|�A�ɤW�u��j�p

	/* �q�o�Ӯɶ��I�}�l�p�ɡA�W�L�ɶ��Ntimeout */
	thisGetBinaryRequest->writeTimePoint();

	/* �o��}�l����� */
	while (!isTransferFinished) {

		if (thisGetBinaryRequest->getSectionElapsedSeconds() > thisGetBinaryRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisGetBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* �T�{���쪺command�O���Oreturn command */
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

					/* �q�o�Ӯɶ��I�}�l�p�ɡA�W�L�ɶ��Ntimeout */
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
								break;	// �p�G�Ӧh���e�A�|�W�L�ʥ]�j�p

						}
					}

					reuqestRetransferMessage->SetContextInJson(messageContext);
					reuqestRetransferMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
					bleAccess->GetBluetoothPhone()->PushOutputMessage(reuqestRetransferMessage);

					/* �q�o�Ӯɶ��I�}�l�p�ɡA�W�L�ɶ��Ntimeout */
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

	/* �i���w�����ɮ� */
	while (!isAckedTransferFinished) {

		if (thisGetBinaryRequest->getSectionElapsedSeconds() > thisGetBinaryRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisGetBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* �T�{���쪺command�O���Oreturn command */
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

					/* �q�o�Ӯɶ��I�}�l�p�ɡA�W�L�ɶ��Ntimeout */
					thisGetBinaryRequest->writeTimePoint();
				}
			}
			thisGetBinaryRequest->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
	if ((thisGetBinaryRequest->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryRequest->callbackScene)) ||	// ����n�ﱼ�o�ӡA���ݭn���ҳ����O�_�s�b
		!thisGetBinaryRequest->isCallbackByScene ||
		onFinish.GetSize() == 0)
		onFinish.TriggerThenClear(&bleBinaryRequestFileSegmentMap);
	else {
		throw BleRequestException(BleResponseCode::Gone);
	}

	return 0;
}

BleRequestMethodType GetBinaryBleRequest::GetBinaryBleRequestMethod::GetMethodType()
{
	return BleRequestMethodType::GetBinary;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::AddOnFinish(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name)
{
	onFinish.Add(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::AddOnFinish(ActionList<int, FileSegmentMap*>* actionsOnFinish)
{
	onFinish.Add(actionsOnFinish);
	return 0;
}

int GetBinaryBleRequest::GetBinaryBleRequestMethod::AddOnGetBinarySuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequestMethod::AddOnGetBinarySuccess() : not implemented.";
	return 0;
}

GetBinaryBleRequest::GetBinaryBleRequest(string fPath, MeteoBluetoothMessage * gMessage, MeteoCommand ackGetCommand, MeteoCommand tCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand) : RegisterType("GetBinaryBleRequest")
{
	LOG(LogLevel::Error) << "GetBinaryBleRequest::GetBinaryBleRequest() : not implemented.";
}

int GetBinaryBleRequest::ChooseCommunicationComponentToPerform()
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequest::ChooseCommunicationComponentToPerform() : not implemented.";
	return 0;
}

int GetBinaryBleRequest::AddOnFinish(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name)
{
	/* �ˬd�O���O��scene�hadd�� */
	if (dynamic_cast<Scene*>(callableObject)) {
		isCallbackByScene &= (dynamic_cast<Scene*>(callableObject) != nullptr);
		if (isCallbackByScene)
			callbackScene = dynamic_cast<Scene*>(callableObject);
		else
			callbackScene = nullptr;
	}

	dynamic_cast<GetBinaryBleRequestMethod*>(requestMethod)->AddOnFinish(callableObject, callback, name);

	return 0;
}

int GetBinaryBleRequest::AddOnFinish(ActionList<int, FileSegmentMap*>* actionsOnFinish)
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequest::AddOnFinish() : not implemented.";
	return 0;
}

int GetBinaryBleRequest::AddOnGetBinarySuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequest::AddOnGetBinarySuccess() : not implemented.";
	return 0;
}

int GetBinaryBleRequest::AddOnGetBinarySuccess(ActionList<int, string>* actionsOnGetBinarySuccess)
{
	LOG(LogLevel::Error) << "int GetBinaryBleRequest::AddOnGetBinarySuccess() : not implemented.";
	return 0;
}
