#include "MeteoBluetoothPhoneV2.h"

#include <thread>
#include <unistd.h>
#include "../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Games/Output/Bluetooths/MeteoFileSegmentBluetoothMessage.h"
#include "Gatt/MeteoGattServerV1.h"
#include <sched.h> 
#include <pthread.h>
#include "../../Framework/Threading/ThreadMaster.h"


using namespace std;
using namespace Desktop::Devices;
using namespace Games::Output::Bluetooths;
using namespace Desktop::Devices::Gatt;
using namespace Framework::Threading;


#ifndef METEO_PROGRAM_VERSION
#define METEO_PROGRAM_VERSION 0x0
#endif


MeteoBluetoothPhoneV2::MeteoBluetoothPhoneV2(PacketConverter<MeteoCommand>* pConverter) : RegisterType("MeteoBluetoothPhone")
{
	packetConverter = pConverter;

}

int MeteoBluetoothPhoneV2::Initialize()
{
	//thisThread = new thread([]() {
	//
	//	LOG(LogLevel::Error) << "call bleconfd";
	//	//FILE* fp = popen(string("sudo /home/pi/bleconfd/bleconfd -d").c_str(), "r");
	//	FILE* fp = popen(string("sudo /home/pi/MeteoBleTestServer/MeteoBleTestServer/MeteoBleTestServer_bass").c_str(), "r");
	//	if (fp == NULL) {
	//		LOG(LogLevel::Error) << "get bleconfd failed";
	//	}
	//	char buf[256];
	//	while (fgets(buf, 255, fp) != NULL)
	//		printf("%s", buf);
	//	pclose(fp);
	//
	//});
	//thisThread->detach();
	//return 0;

	bluetoothState = new InputState();
	bluetoothState->SetBluetoothState(new BluetoothState());


	thisThread = new thread(&MeteoBluetoothPhoneV2::work, this);
	sleepTimeInMilliSecond = 0;

	ThreadMaster::GetInstance().AddSimpleThread(this);

	int policy = SCHED_RR;
	struct sched_param param;
	memset(&param, 0, sizeof(param));
	param.sched_priority = sched_get_priority_max(policy);
	pthread_setschedparam(thisThread->native_handle(), policy, &param);

	thisThread->detach();
	return 0;
}

InputState * MeteoBluetoothPhoneV2::GetBluetoothState()
{
	if (inputBytes.size() > 0)
	if (inputByteMutex.try_lock()) {

		vector<pair<const char*, int>> tempPackets;
		tempPackets.assign(inputBytes.begin(), inputBytes.end());
		inputBytes.clear();
		inputByteMutex.unlock();

		for (int i = 0; i < tempPackets.size(); i++) {
			ConvertPacketToMessage(tempPackets[i].first, tempPackets[i].second);
		}
	}



	if (bluetoothState->GetBluetoothState()->CheckIsEmpty())
		return nullptr;

	InputState* returnValue = bluetoothState;

	unique_lock<mutex> uLock(bluetoothStateMutex);
	bluetoothState = new InputState();
	bluetoothState->SetBluetoothState(new BluetoothState());
	uLock.unlock();

	return returnValue;
}

int MeteoBluetoothPhoneV2::PushOutputMessage(BluetoothMessage * outputMessage)
{
	if (!getIsReady()) {
		LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV2::PushOutputMessage() : phone not ready.";
		return 0;
	}

	if (gattServer == nullptr) {
		LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV2::PushOutputMessage() : server not ready.";
		return 0;
	}

	/* 如果write queue塞了很多message，就先不要丟file message，免得一般message丟不出去 */
	//if (dynamic_cast<MeteoFileSegmentBluetoothMessage*>(outputMessage)) {
	//	if (gattServer->GetClient() != nullptr) {
	//		if (gattServer->GetClient()->GetWriteQueueLength() > outputBufferThreshold)
	//			return -1;
	//	}
	//}

	char buffer[256] = { 0 };

	int size = packetConverter->ConvertToByteArray(outputMessage, buffer, mtu);//??

	if (size != -1) {
		gattServer->GetClient()->SendNotification(buffer, size);

		//for (int i = 0; i < size; i++) {
		//	printf("%x ", buffer[i]);
		//}
		//printf("\n");
	}
	else {
		LOG(LogLevel::Error) << "MeteoBluetoothPhoneV2::PushOutputMessage() : message size over mtu [" << mtu << "].";
	}
		
	return 0;
}

int MeteoBluetoothPhoneV2::AddOnConnect(MtoObject * callableObject, function<int()> callback, string name)
{
	onConnect.Add(callableObject, callback, name);
	return 0;
}

int MeteoBluetoothPhoneV2::AddOnDisconnect(MtoObject * callableObject, function<int()> callback, string name)
{
	onDisconnect.Add(callableObject, callback, name);
	return 0;
}

bool MeteoBluetoothPhoneV2::getIsReady()
{
	return isConnected && isFirstPacketSent;
}

int MeteoBluetoothPhoneV2::work()
{

	while (!exitRequested) {

		try
		{
			LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV2::work() : start listening.";
			gattServer = new MeteoGattServerV1();
			GattClient* gattClient = gattServer->Listen();

			//gattClient->SetDataHandler(std::bind(&MeteoGattServerV1::OnIncomingMessage, dynamic_cast<MeteoGattServerV1*>(gattServer), std::placeholders::_1, std::placeholders::_2));
			gattClient->SetDataHandler(std::bind(&MeteoBluetoothPhoneV2::handleNewPacket, this, std::placeholders::_1, std::placeholders::_2));

			isConnected = true;
			onConnect.Trigger();

			gattServer->Run(gattClient);

			GattServer* toDelete = gattServer;
			gattServer = nullptr;
			delete toDelete;
			isConnected = false;
			isFirstPacketSent = false;
		}
		catch (std::exception const& err)
		{
			LOG(LogLevel::Error) << "MeteoBluetoothPhoneV2::work() : gatt get error:" << err.what();
			GattServer* toDelete = gattServer;
			gattServer = nullptr;
			delete toDelete;
			isConnected = false;
			isFirstPacketSent = false;
			continue;
		}

		onDisconnect.Trigger();

	}
	return 0;


}

int MeteoBluetoothPhoneV2::pushBluetoothState(BluetoothMessage * btMessage)
{
	LOG(LogLevel::Depricated) << "MeteoBluetoothPhoneV1::pushBluetoothState() : got new bt command.";

	unique_lock<mutex> uLock(bluetoothStateMutex);

	bluetoothState->GetBluetoothState()->AddMessage(btMessage);

	return 0;
}

int MeteoBluetoothPhoneV2::handleNewPacket(const char * packet, int length)
{
	isFirstPacketSent = true;

	char* buffer = new char[length];
	memcpy(buffer, packet, length);
	
	unique_lock<mutex> uLock(inputByteMutex);
	inputBytes.push_back(pair<const char*, int>(buffer, length));

	return 0;
	
}

int MeteoBluetoothPhoneV2::ConvertPacketToMessage(const char * packet, int length)
{
	PacketType packetType = packetConverter->CheckPacketType(packet, length);

	/* 讀取韌體版號 */
	if (packetType == PacketType::ReadFirmwareVersion) {
		// TODO: 做一個firmware version專用的bt message class
		if (length != 4)
			goto OUT;

		if (gattServer == nullptr)
			goto OUT;

		if (gattServer->GetClient() == nullptr)
			goto OUT;

		LOG(LogLevel::Fine) << "MeteoBluetoothPhoneV2::ConvertPacketToMessage() : get read firmware version message.";

		char buffer[8] = { 0 };
		unsigned int command = 0x110000;// MeteoCommand::ReturnFirmwareVersion
		unsigned int version = METEO_PROGRAM_VERSION;

		memcpy(buffer, &command, sizeof(command));
		memcpy(buffer + sizeof(command), &version, sizeof(version));

		gattServer->GetClient()->SendNotification(buffer, 8);
		isFirstPacketSent = true;
	}
	else if (packetType == PacketType::Json) {

		if (!getIsReady()) {
			goto OUT;
		}

		BluetoothMessage* btMessage = packetConverter->ConvertToBluetoothMessage(packet, length);

		if (btMessage == nullptr) {
			LOG(LogLevel::Error) << "MeteoBluetoothPhoneV2::ConvertPacketToMessage() : convert to bt command failed.";
			goto OUT;
		}

		LOG(LogLevel::Fine) << "MeteoBluetoothPhoneV2::ConvertPacketToMessage() : massage:" << btMessage->ToString();

		if (btMessage != nullptr)
			pushBluetoothState(btMessage);
	}
	else if (packetType == PacketType::File) {

		LOG(LogLevel::Fine) << "MeteoBluetoothPhoneV2::ConvertPacketToMessage() : file segment massage.";

		if (!getIsReady())
			goto OUT;

		BluetoothMessage* btMessage = packetConverter->ConvertToFile(packet, length);

		if (btMessage != nullptr)
			pushBluetoothState(btMessage);
	}
	else if (packetType == PacketType::AckFile) {

		LOG(LogLevel::Fine) << "MeteoBluetoothPhoneV2::ConvertPacketToMessage() : ack file segment massage.";

		if (!getIsReady())
			goto OUT;

		BluetoothMessage* btMessage = packetConverter->ConvertToAckFileMessage(packet, length);

		if (btMessage != nullptr)
			pushBluetoothState(btMessage);
	}
	else if (packetType == PacketType::None) {
		// 封包壞掉，直接丟掉，不用刪因為return以後外面會刪
		// packetConverter->CleanBuffer();
		LOG(LogLevel::Warning) << "MeteoBluetoothPhoneV2::ConvertPacketToMessage() : got error packet.";
	}

OUT:
	delete[] packet;

	return 0;
}

int MeteoBluetoothPhoneV2::setMtu(int m)
{
	if (mtu > maxMtu) {
		LOG(LogLevel::Error) << "MeteoBluetoothPhoneV2::setMtu() : new mtu [" << mtu << "] over max mtu [" << maxMtu << "].";
		return -1;
	}
	mtu = m;
	return 0;
}
