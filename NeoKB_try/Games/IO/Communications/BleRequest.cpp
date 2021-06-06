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



BleRequest::BleRequest() : RegisterType("BleRequest")
{
}

BleRequest::~BleRequest()
{
	for (int i = 0; i < inputRawMessages.size(); i++) {
		delete inputRawMessages[i];
	}
	inputRawMessages.clear();

	for (int i = 0; i < inputRawMessagesBuffer.size(); i++) {
		delete inputRawMessagesBuffer[i];
	}
	inputRawMessagesBuffer.clear();
}

int BleRequest::ChooseCommunicationComponentToPerform()
{

	LOG(LogLevel::Debug) << "int BleRequest::ChooseCommunicationComponentToPerform() : [" << GetTypeName() << "] request finding component.";

	map<string, deque<CommunicationRequest*>*>::iterator it;

	for (it = acceptedCommunicationComponentRequestQueues.begin(); it != acceptedCommunicationComponentRequestQueues.end(); ++it) {
		if (it->first == "BleAccess") {

			LOG(LogLevel::Debug) << "int BleRequest::ChooseCommunicationComponentToPerform() : [" << GetTypeName() << "] component found.";
			it->second->push_back(this);
		}
	}


	return 0;
}

int BleRequest::Perform(CommunicationComponent * cComponent)
{
	communicationComponent = cComponent;

	BleAccess* bleAccess = dynamic_cast<BleAccess*>(communicationComponent);
	// ��ble access��raw message��i�ӡA�n���ڭ��ˬd���S��ack��return
	bleAccess->RegisterBleRequest(this);

	requestStartTime = system_clock::now();
	exitRequested = false;

	// preform ���T�X�h�M�ᵥ�^��

	try {
		requestMethod->PerformAndWait(this);
	}
	catch (exception& e) {
		// ���槹���H��N����ble access��raw message��i��
		bleAccess->UnregisterBleRequest(this);
		throw e;
	}

	// ���槹���H��N����ble access��raw message��i��
	bleAccess->UnregisterBleRequest(this);

	// �d�ݦ��G���S�����A�������ܴNthrow exception
	checkAndProcessFailure();
	
	
	// �p�G���X�{���~�A�|��exception�A�N���|����on success�C�o��n�`�Nrequest�b���槹�H�ᤣ�ઽ���R���A�n�T�w�Ҧ�task���]���~��R
	communicationComponent->GetScheduler()->AddTask([=]() {
		/* �ˬdScene�O�_�٦s�b�A�s�b�~����� */
		if ((isCallbackByScene && SceneMaster::GetInstance().CheckScene(callbackScene)) ||
			!isCallbackByScene ||
			onSuccess.GetSize() == 0)
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
		if (requestMethod->GetMethodType() == BleRequestMethodType::GetBinary) {
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

int BleRequest::checkAndProcessFailure()
{
	return 0;
}

int BleRequest::fail(exception * e)
{

	if (dynamic_cast<CommunicationRequestException*>(e)) {

	}

	LOG(LogLevel::Debug) << "int BleRequest::fail() : not implemented.";
	return 0;
}
