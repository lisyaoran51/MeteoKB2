#include "GetBinaryBleRequestHandler.h"

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




GetBinaryBleRequestHandler::GetBinaryBleRequestHandler(string dPath, string fName, MeteoCommand gCommand, MeteoCommand ackGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand) : RegisterType("GetBinaryBleRequestHandler")
{
	GetBinaryBleRequestHandlerMethod* method = new GetBinaryBleRequestHandlerMethod(
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

int GetBinaryBleRequestHandler::AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	dynamic_cast<GetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnSuccess(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequestHandler::AddOnSuccess(ActionList<int, string>* callback)
{
	dynamic_cast<GetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnSuccess(callback);
	return 0;
}

int GetBinaryBleRequestHandler::AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name)
{
	dynamic_cast<GetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnFail(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequestHandler::AddOnFail(ActionList<int, string>* callback)
{
	dynamic_cast<GetBinaryBleRequestHandlerMethod*>(requestMethod)->AddOnFail(callback);
	return 0;
}

int GetBinaryBleRequestHandler::fail(exception * e)
{
	requestMethod->Fail(this);
	return 0;
}

int GetBinaryBleRequestHandler::success()
{
	requestMethod->Success(this);
	return 0;
}

GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::GetBinaryBleRequestHandlerMethod(string dPath, string fName, MeteoCommand gCommand, MeteoCommand aGetCommand, MeteoCommand tCommand, MeteoCommand aTransferCommand, MeteoCommand fCommand, MeteoCommand rRetransferCommand, MeteoCommand aFinishCommand)
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

}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait(BleRequest * thisRequest)
{
	// #-- 1 �}�l
	GetBinaryBleRequestHandler* thisGetBinaryBleRequestHandler = dynamic_cast<GetBinaryBleRequestHandler*>(thisRequest);

	/* ��ثe���Ū�mtu */
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

		/* �o�q�g�o�ܪ��A�\��N�u�O�⦬�쪺ack�ᵹrequest�Ӥw */
		unique_lock<mutex> uLock(thisGetBinaryBleRequestHandler->rawMessageMutex);
		while (thisGetBinaryBleRequestHandler->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisGetBinaryBleRequestHandler->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == getCommand) {

				/* �d�ݬO�_���ɮ� */
				string requestFileName = dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["FileName"].get<string>();

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

		this_thread::sleep_for(std::chrono::milliseconds(10));

	}

	int mtu = bleAccess->GetMtu();

	int binarySegmentSize = mtu - 28;
	if (binarySegmentSize <= 0) {
		LOG(LogLevel::Error) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : mtu size " << mtu << " too small.";
		throw BleRequestException(BleResponseCode::MtuTooSmall);
		//throw logic_error("BleRequest::PostBinaryBleRequestMethod::PerformAndWait(): mtu size too small.");
	}

	/* Ū�ɨÿ�Jmap�� */
	fstream file(directoryPath + string("/") + fileName, ios::binary);

	FileSegmentMap fileSegmentMap(binarySegmentSize);
	fileSegmentMap.ReadFile(&file);
	fileSegmentMap.fileName = fileName;

	// ���U�Ӽg�@��builder�i�H��file segment�]��message�ᵹbluetooth phone�A�᧹�H��A������ʧ@�pfinish command�Bretransfer����

	vector<MeteoFileSegmentBluetoothMessage*> fileSegmentMessages;

	for (int i = 0; i < fileSegmentMap.segmentAmount; i++) {
		char* tempFileSegment = nullptr;
		int tempFileSegmentSize = 0;
		tempFileSegmentSize = fileSegmentMap.GetFileSegment(i, &tempFileSegment);

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
		LOG(LogLevel::Error) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : file segment resolve to no file segment.";
		throw BleRequestException(BleResponseCode::TransferToFileSegmentsFailed);
	}

	// #-- 2 ����

	/* �o��n�����ɮסA�C��@���ɮץH��n�����ˬd�O�_����Ack�A�~��A��U�@���ɮ� */

	int tempSendFileSegmentNumber = 0;
	double transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
	bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[0]);


	while (1) {

		if (thisGetBinaryBleRequestHandler->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		/* �ˬdScene�O�_�٦s�b�A���s�b�N�������X */
		if (thisGetBinaryBleRequestHandler->isCallbackByScene && !SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequestHandler->callbackScene))
		{
			throw BleRequestException(BleResponseCode::Gone);
		}

		// �p�G�Ӥ[�S����ack�A�N������U�@��file segment
		if (thisGetBinaryBleRequestHandler->getSectionElapsedSeconds() - transferTempFileSegmentElapsedSeconds > sendFileSegmentTimeout) {

			LOG(LogLevel::Warning) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : no receive ack file segment [" << tempSendFileSegmentNumber << "]. Pass.";

			// �w�᧹�Ҧ�file segment�A�N�������X���U�@�ӨB�J
			if (tempSendFileSegmentNumber == fileSegmentMap.segmentAmount - 1)
				break;

			tempSendFileSegmentNumber++;
			transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
			bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[tempSendFileSegmentNumber]);
		}

		/* �o�q�g�o�ܪ��A�\��N�u�O�⦬�쪺ack�ᵹrequest�Ӥw */
		unique_lock<mutex> uLock(thisGetBinaryBleRequestHandler->rawMessageMutex);
		while (thisGetBinaryBleRequestHandler->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisGetBinaryBleRequestHandler->inputRawMessages.back();

			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetCommand() == ackTransferCommand &&
				dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetFileName() == fileSegmentMap.fileName) {

				MeteoAckFileSegmentBluetoothMessage* ackFileSegmentMessage = dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message);
				if (ackFileSegmentMessage->GetOrder() == tempSendFileSegmentNumber) {

					tempSendFileSegmentNumber++;
					transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
					bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[tempSendFileSegmentNumber]);
				}
				else if (ackFileSegmentMessage->GetOrder() < tempSendFileSegmentNumber) {
					// no-op
				}
				else {
					LOG(LogLevel::Error) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : ack file segment number worng -- [" << ackFileSegmentMessage->GetOrder() << "]. Temp file Segment Number is [" << tempSendFileSegmentNumber << "].";
				}
			}
			}
			thisGetBinaryBleRequestHandler->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

	}

	// #-- 3 �T�{

	MeteoContextBluetoothMessage* checkFinishMessage = new MeteoContextBluetoothMessage(finishCommand);
	json messageContext;
	messageContext["FileName"] = fileSegmentMap.fileName;
	checkFinishMessage->SetContextInJson(messageContext);
	bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

	// ���ݹ���z�Ҧ��ݭn���Ǫ�file segment
	this_thread::sleep_for(std::chrono::milliseconds(200));

	bool isFinished = false;
	bool isRetransferred = false;

	/* �q�o�Ӯɶ��I�}�l�p�ɡA�W�L�ɶ��Ntimeout */
	thisGetBinaryBleRequestHandler->writeTimePoint();

	// �ثe���Ǩ� retransferOrder �����ĴX�� file segment
	int tempSendRetransferOrderIndex = -1;
	transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();

	while (1) {

		if (thisGetBinaryBleRequestHandler->getSectionElapsedSeconds() > thisGetBinaryBleRequestHandler->timeout) {
			throw BleRequestException(BleResponseCode::RequestTimeout);
		}

		if (thisGetBinaryBleRequestHandler->exitRequested) {
			throw BleRequestException(BleResponseCode::ExitRequested);
		}

		// �p�G�Ӥ[�S����ack�A�N������U�@��file segment
		if (tempSendRetransferOrderIndex >= 0 &&
			thisGetBinaryBleRequestHandler->getSectionElapsedSeconds() - transferTempFileSegmentElapsedSeconds > sendFileSegmentTimeout) {

			LOG(LogLevel::Warning) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : no receive ack retransfer file segment [" << tempSendRetransferOrderIndex << " / " << retransferOrders.size() << "] which file sgement is [" << retransferOrders[tempSendRetransferOrderIndex] << "]. Pass.";

			// �w�᧹�Ҧ�file segment�A�N�������X���U�@�ӨB�J
			if (tempSendRetransferOrderIndex == retransferOrders.size() - 1) {
				LOG(LogLevel::Debug) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : retransfer over.";
				bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

				// ���ݹ���z�Ҧ��ݭn���Ǫ�file segment
				this_thread::sleep_for(std::chrono::milliseconds(200));
				transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
			}
			else {

				tempSendRetransferOrderIndex++;
				transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
				bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);

			}

		}

		/* �o�q�g�o�ܪ��A�\��O�ˬd���S����������A�����ܴN���}�A�S���N���ǡA���ǧ��n�A�ˬd�@�����S������ */
		unique_lock<mutex> uLock(thisGetBinaryBleRequestHandler->rawMessageMutex);
		while (thisGetBinaryBleRequestHandler->inputRawMessages.size() > 0) {
			MeteoBluetoothMessage* message = thisGetBinaryBleRequestHandler->inputRawMessages.back();

			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == ackFinishCommand) {

				/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
				if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["Status"].get<int>() == 0) {
					LOG(LogLevel::Debug) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : transfer success.";
					break;
				}

				LOG(LogLevel::Debug) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : transfer not success.";

			}
			else if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetCommand() == requestRetransferCommand) {
			if (dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["FileName"].get<string>() == fileSegmentMap.fileName) {
				int retransferOrder = dynamic_cast<MeteoContextBluetoothMessage*>(message)->GetContextInJson()["Order"].get<int>();

				retransferOrders.push_back(retransferOrder);

				if (retransferOrders.size() == 1) {
					tempSendRetransferOrderIndex = 0;
					bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);
				}

			}
			}
			}

			/* ��Dack�H��A�~�򭫶� */
			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)) {
			if (dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetCommand() == ackTransferCommand &&
				dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetFileName() == fileSegmentMap.fileName) {

				int ackIndex = -1;
				for (int i = 0; i < retransferOrders.size(); i++) {
					if (retransferOrders[i] == dynamic_cast<MeteoAckFileSegmentBluetoothMessage*>(message)->GetOrder()) {
						ackIndex = i;
					}
				}

				if (ackIndex != -1 && ackIndex != retransferOrders.size() - 1 && ackIndex == tempSendRetransferOrderIndex) {

					tempSendRetransferOrderIndex++;
					transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
					bleAccess->GetBluetoothPhone()->PushOutputMessage(fileSegmentMessages[retransferOrders[tempSendRetransferOrderIndex]]);

				}
				else if (ackIndex == retransferOrders.size() - 1) {
					bleAccess->GetBluetoothPhone()->PushOutputMessage(checkFinishMessage);

					// ���ݹ���z�Ҧ��ݭn���Ǫ�file segment
					this_thread::sleep_for(std::chrono::milliseconds(200));

					transferTempFileSegmentElapsedSeconds = thisGetBinaryBleRequestHandler->getSectionElapsedSeconds();
				}
				else if (ackIndex > tempSendRetransferOrderIndex) {
					LOG(LogLevel::Error) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::PerformAndWait() : receive error ack retransfer index [" << ackIndex << "/" << retransferOrders.size() << "]. temp index is [" << tempSendRetransferOrderIndex << "]";
				}
			}
			}

			thisGetBinaryBleRequestHandler->inputRawMessages.pop_back();
			delete message;
		}

		uLock.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	// ����

	return 0;

}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::Fail(BleRequest* thisRequest)
{
	LOG(LogLevel::Debug) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::Fail() : fail to handle get file request.";

	//if (fileName == "")
	//	return 0;

	GetBinaryBleRequestHandler* thisGetBinaryBleRequestHandler = dynamic_cast<GetBinaryBleRequestHandler*>(thisRequest);
	if ((thisGetBinaryBleRequestHandler->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequestHandler->callbackScene)) ||
		!thisGetBinaryBleRequestHandler->isCallbackByScene)
		onFail.TriggerThenClear(fileName);

	return 0;
}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::Success(BleRequest* thisRequest)
{
	LOG(LogLevel::Debug) << "GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::Fail() : handle get file request success.";

	//if (fileName == "")
	//	return 0;

	GetBinaryBleRequestHandler* thisGetBinaryBleRequestHandler = dynamic_cast<GetBinaryBleRequestHandler*>(thisRequest);
	if ((thisGetBinaryBleRequestHandler->isCallbackByScene && SceneMaster::GetInstance().CheckScene(thisGetBinaryBleRequestHandler->callbackScene)) ||
		!thisGetBinaryBleRequestHandler->isCallbackByScene)
		onSuccess.TriggerThenClear(fileName);
	return 0;
}

BleRequestMethodType GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::GetMethodType()
{
	return BleRequestMethodType::GetBinaryHandler;
}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::AddOnFail(MtoObject * callableObject, function<int(string)> callback, string name)
{
	onFail.Add(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::AddOnFail(ActionList<int, string>* callback)
{
	onFail.Add(callback);
	return 0;
}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::AddOnSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	onSuccess.Add(callableObject, callback, name);
	return 0;
}

int GetBinaryBleRequestHandler::GetBinaryBleRequestHandlerMethod::AddOnSuccess(ActionList<int, string>* callback)
{
	onSuccess.Add(callback);
	return 0;
}
