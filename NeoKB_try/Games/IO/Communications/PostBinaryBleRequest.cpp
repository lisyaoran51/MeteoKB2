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

		bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessage);
	}

	/* �����ɮץ�����X�h�A�A�e�X�ˬd�T���A�й���ˬd�O�_�������� */
	while (!bleAccess->GetBluetoothPhone()->CheckFileSegmentMessageOutputClear()) {
		if (thisPostBinaryRequest->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}
		// TODO: �p�G���Ӥ[�A�N��exception
	}

	MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
	json messageContext;
	messageContext["FileName"] = bleBinaryRequestFileSegmentMap.fileName;
	checkFinishMessage->SetContextInJson(messageContext);
	bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);


	bool isFinished = false;
	bool isRetransferred = false;

	/* �q�o�Ӯɶ��I�}�l�p�ɡA�W�L�ɶ��Ntimeout */
	thisPostBinaryRequest->writeTimePoint();

	while (1) {

		if (thisPostBinaryRequest->getSectionElapsedSeconds() > thisPostBinaryRequest->timeout) {
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
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackFinishCommand) {


					/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
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

						// TODO ���ǳo��file segment
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

				// TODO: �p�G���Ӥ[�A�N��exception
			}


			/* �q�o�Ӯɶ��I�}�l�p�ɡA�W�L�ɶ��Ntimeout */
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

	/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
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
