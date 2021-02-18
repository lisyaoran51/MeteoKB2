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



