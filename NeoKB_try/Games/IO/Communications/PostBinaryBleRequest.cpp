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
	// #-- 1 �}�l
	PostBinaryBleRequest* thisPostBinaryRequest = dynamic_cast<PostBinaryBleRequest*>(thisRequest);

	/* ��ثe���Ū�mtu */
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

		/* �o�q�g�o�ܪ��A�\��N�u�O�⦬�쪺ack�ᵹrequest�Ӥw */
		unique_lock<mutex> uLock(thisPostBinaryRequest->rawMessageMutex);
		while (thisPostBinaryRequest->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisPostBinaryRequest->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackPostCommand) {

					/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
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

	/* Ū�ɨÿ�Jmap�� */
	fstream file(filePath, ios::binary);

	FileSegmentMap bleBinaryRequestFileSegmentMap(binarySegmentSize);
	bleBinaryRequestFileSegmentMap.ReadFile(&file);
	vector<string> splitPath = StringSplitter::Split(filePath, "/");
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
	}

	if (fileSegmentMessages.size() == 0) {
		LOG(LogLevel::Error) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : file segment resolve to no file segment.";
		throw BleRequestException(BleResponseCode::TransferToFileSegmentsFailed);
	}

	// #-- 2 ����

	/* �o��n�����ɮסA�C��@���ɮץH��n�����ˬd�O�_����Ack�A�~��A��U�@���ɮ� */

	int tempSendFileSegmentNumber = 0;
	double transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
	bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[0]);


	while (1) {

		if (thisPostBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* �ˬdScene�O�_�٦s�b�A���s�b�N�������X */
		if (thisPostBinaryRequest->isCallbackByScene && !SceneMaster::GetInstance().CheckScene(thisPostBinaryRequest->callbackScene))
		{
			throw BleRequestException(BleResponseCode::Gone);
		}

		// �p�G�Ӥ[�S����ack�A�N������U�@��file segment
		if (thisPostBinaryRequest->getSectionElapsedSeconds() - transferTempFileSegmentElapsedSeconds > sendFileSegmentTimeout) {

			LOG(LogLevel::Warning) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : no receive ack file segment [" << tempSendFileSegmentNumber << "]. Pass.";

			// �w�᧹�Ҧ�file segment�A�N�������X���U�@�ӨB�J
			if (tempSendFileSegmentNumber == bleBinaryRequestFileSegmentMap.segmentAmount - 1)
				break;

			tempSendFileSegmentNumber++;
			transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
			bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[tempSendFileSegmentNumber]);
		}

		/* �o�q�g�o�ܪ��A�\��N�u�O�⦬�쪺ack�ᵹrequest�Ӥw */
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
	
	// #-- 3 �T�{

	MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
	json messageContext;
	messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
	checkFinishMessage->SetContextInJson(messageContext);
	bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

	// ���ݹ���z�Ҧ��ݭn���Ǫ�file segment
	this_thread::sleep_for(std::chrono::milliseconds(200));

	bool isFinished = false;
	bool isRetransferred = false;

	/* �q�o�Ӯɶ��I�}�l�p�ɡA�W�L�ɶ��Ntimeout */
	thisPostBinaryRequest->writeTimePoint();

	// �ثe���Ǩ� retransferOrder �����ĴX�� file segment
	int tempSendRetransferOrderIndex = -1;
	transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();

	while (1) {

		if (thisPostBinaryRequest->getSectionElapsedSeconds() > thisPostBinaryRequest->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisPostBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		// �p�G�Ӥ[�S����ack�A�N������U�@��file segment
		if (tempSendRetransferOrderIndex >= 0 &&
			thisPostBinaryRequest->getSectionElapsedSeconds() - transferTempFileSegmentElapsedSeconds > sendFileSegmentTimeout) {

			LOG(LogLevel::Warning) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : no receive ack retransfer file segment [" << tempSendRetransferOrderIndex << " / " << retransferOrders.size() << "] which file sgement is [" << retransferOrders[tempSendRetransferOrderIndex] << "]. Pass.";

			// �w�᧹�Ҧ�file segment�A�N�������X���U�@�ӨB�J
			if (tempSendRetransferOrderIndex == retransferOrders.size() - 1) {
				LOG(LogLevel::Debug) << "BleRequest::PostBinaryBleRequestMethod::PerformAndWait() : retransfer over.";
				bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

				// ���ݹ���z�Ҧ��ݭn���Ǫ�file segment
				this_thread::sleep_for(std::chrono::milliseconds(200));
				transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
			}
			else {

				tempSendRetransferOrderIndex++;
				transferTempFileSegmentElapsedSeconds = thisPostBinaryRequest->getSectionElapsedSeconds();
				bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);

			}

		}

		/* �o�q�g�o�ܪ��A�\��O�ˬd���S����������A�����ܴN���}�A�S���N���ǡA���ǧ��n�A�ˬd�@�����S������ */
		unique_lock<mutex> uLock(thisPostBinaryRequest->rawMessageMutex);
		while (thisPostBinaryRequest->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisPostBinaryRequest->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackFinishCommand) {

				/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
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

			/* ��Dack�H��A�~�򭫶� */
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

						// ���ݹ���z�Ҧ��ݭn���Ǫ�file segment
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

	/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
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
	/* �ˬd�O���O��scene�hadd�� */
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
	/* �ˬd�O���O��scene�hadd�� */
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
