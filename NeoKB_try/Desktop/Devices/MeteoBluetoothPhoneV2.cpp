#include "MeteoBluetoothPhoneV2.h"

#include <thread>
#include <unistd.h>
#include "../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Games/Output/Bluetooths/MeteoFileSegmentBluetoothMessage.h"
#include "Gatt/MeteoGattServerV1.h"
#include <sched.h> 
#include <pthread.h>


using namespace std;
using namespace Desktop::Devices;
using namespace Games::Output::Bluetooths;
using namespace Desktop::Devices::Gatt;


#ifndef METEO_PROGRAM_VERSION
#define METEO_PROGRAM_VERSION 0x0
#endif




MeteoBluetoothPhoneV2::MeteoBluetoothPhoneV2(PacketConverter<MeteoCommand>* pConverter)
{
	packetConverter = pConverter;

}

int MeteoBluetoothPhoneV2::Initialize()
{
	bluetoothState = new InputState();
	bluetoothState->SetBluetoothState(new BluetoothState());


	thread t(&MeteoBluetoothPhoneV2::work, this);

	int policy = SCHED_RR;
	struct sched_param param;
	memset(&param, 0, sizeof(param));
	param.sched_priority = sched_get_priority_max(policy);
	pthread_setschedparam(t.native_handle(), policy, &param);

	t.detach();
	return 0;
}

InputState * MeteoBluetoothPhoneV2::GetBluetoothState()
{
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


	//client = init_server(0x1);

	isConnected = true;

	while (!exitRequested) {
		readBluetooth();
		writeBluetooth();

		if (!lastRunReceived && !lastRunSended)
			this_thread::sleep_for(chrono::milliseconds(10));

	}
	return 0;
}

int MeteoBluetoothPhoneV2::readBluetooth()
{
	if (gattServer == nullptr) {
		// TODO: clear buffer
		return -1;
	}

	LOG(LogLevel::Depricated) << "MeteoBluetoothPhoneV1::readBluetooth() : start reading.";
	if (lastRunReceived)
		memset(bufferIn, 0, sizeof(bufferIn));

	int bytes_read = read(client, bufferIn, sizeof(bufferIn));
	if (bytes_read > 0) {
		lastRunReceived = true;

		if (bytes_read == 32768)
			LOG(LogLevel::Error) << "MeteoBluetoothPhoneV1::readBluetooth() : buffer overflow.";

		char** packets = new char*[128];
		int* packetLengths = new int[128];

		LOG(LogLevel::Depricated) << "MeteoBluetoothPhoneV1::readBluetooth() : spliting packet.";

		int packetCount = packetConverter->SplitPacket(bufferIn, bytes_read, packets, packetLengths);

		LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV1::readBluetooth() : packets [" << packetCount << "] from [" << bytes_read << "] bytes.";
		for (int i = 0; i < packetCount; i++) {

			PacketStatus packetStatus = packetConverter->CheckPacketStatus(packets[i], packetLengths[i]);

			if (packetStatus == PacketStatus::Fine) {
				handleNewPacket(packets[i], packetLengths[i]);
			}
			else {
				LOG(LogLevel::Error) << "MeteoBluetoothPhoneV1::readBluetooth() : packet error with status [" << (int)packetStatus << "].";
				// 處理錯誤的封包
			}

			delete[] packets[i];
		}


		delete[] packets;
		delete[] packetLengths;

	}
	else
		lastRunReceived = false;



	return 0;
}

int MeteoBluetoothPhoneV2::writeBluetooth()
{
	if (gattServer == nullptr) {
		// TODO: clear buffer
		return -1;
	}


	LOG(LogLevel::Debug) << "MeteoPacketConverterV2::writeBluetooth() : start writing.";
	if (lastRunSended)
		memset(bufferOut, 0, sizeof(bufferOut));

	if (outputMessages.size() > 0/* || outputMessagesToRewrite.size() > 0*/) {

		for (int i = 0; i < outputMessages.size(); i++) {

			int packetCount = packetConverter->GetCountOfPacket(outputMessages[i]);

			for (int j = 0; j < packetCount; j++) {

				int bytes_write = packetConverter->ConvertToByteArray(outputMessages[i], j, bufferOut, 1024);

				write(client, bufferOut, bytes_write);

				memset(bufferOut, 0, sizeof(bufferOut));
			}

			/* 如果是傳送檔案的話，要檢查有沒有傳送成功 */
			if (packetConverter->CheckCommandType(outputMessages[i]) == PacketType::File) {
				//outputMessagesToRewrite.push_back(pair<int, MeteoOutputFileBluetoothCommand*>(OutputMessageToRewriteCountdown, dynamic_cast<MeteoOutputFileBluetoothCommand*>(outputMessages[i])));
			}
			else {
				delete outputMessages[i];
			}

			outputMessages.erase(outputMessages.begin() + i);
			i--;
		}

		lastRunSended = true;
	}
	else
		lastRunSended = false;


	return 0;
}

int MeteoBluetoothPhoneV2::pushBluetoothState(BluetoothMessage * btMessage)
{
	LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV1::pushBluetoothState() : got new bt command.";

	unique_lock<mutex> uLock(bluetoothStateMutex);

	bluetoothState->GetBluetoothState()->AddMessage(btMessage);

	return 0;
}

int MeteoBluetoothPhoneV2::handleNewPacket(const char * packet, int length)
{

	LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV2::handleNewPacket() : length [" << length << "].";

	PacketType packetType = packetConverter->CheckPacketType(packet, length);

	/* 讀取韌體版號 */
	if (packetType == PacketType::ReadFirmwareVersion) {
		// TODO: 做一個firmware version專用的bt message class
		if (length != 4)
			return 0;

		if (gattServer == nullptr)
			return 0;

		if (gattServer->GetClient() == nullptr)
			return 0;

		LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV2::handleNewPacket() : get read firmware version message.";

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
			return 0;
		}

		BluetoothMessage* btMessage = packetConverter->ConvertToBluetoothMessage(packet, length);

		if (btMessage == nullptr) {
			LOG(LogLevel::Error) << "MeteoBluetoothPhoneV2::handleNewPacket() : convert to bt command failed.";
			return 0;
		}

		LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV2::handleNewPacket() : massage:" << btMessage->ToString();

		if (btMessage != nullptr)
			pushBluetoothState(btMessage);
	}
	else if (packetType == PacketType::File) {

		LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV2::handleNewPacket() : file segment massage.";

		if (!getIsReady())
			return 0;

		BluetoothMessage* btMessage = packetConverter->ConvertToFile(packet, length);

		if (btMessage != nullptr)
			pushBluetoothState(btMessage);
	}
	else if (packetType == PacketType::AckFile) {

		LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV2::handleNewPacket() : ack file segment massage.";

		if (!getIsReady())
			return 0;

		BluetoothMessage* btMessage = packetConverter->ConvertToAckFileMessage(packet, length);

		if (btMessage != nullptr)
			pushBluetoothState(btMessage);
	}
	else if (packetType == PacketType::None) {
		// 封包壞掉，直接丟掉，不用刪因為return以後外面會刪
		// packetConverter->CleanBuffer();
		LOG(LogLevel::Info) << "MeteoBluetoothPhoneV1::handleNewPacket() : got error packet.";
	}

	return 0;
}

int MeteoBluetoothPhoneV2::setMtu(int m)
{
	if (mtu > maxMtu) {
		LOG(LogLevel::Error) << "MeteoBluetoothPhoneV1::setMtu() : new mtu [" << mtu << "] over max mtu [" << maxMtu << "].";
		return -1;
	}
	mtu = m;
	return 0;
}
