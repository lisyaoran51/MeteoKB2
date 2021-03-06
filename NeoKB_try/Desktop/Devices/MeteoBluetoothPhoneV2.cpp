#include "MeteoBluetoothPhoneV2.h"

#include <thread>
#include <unistd.h>
#include "../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"


using namespace std;
using namespace Desktop::Devices;
using namespace Games::Output::Bluetooths;




MeteoBluetoothPhoneV2::MeteoBluetoothPhoneV2(PacketConverter<MeteoCommand>* pConverter)
{
	packetConverter = pConverter;

}

int MeteoBluetoothPhoneV2::Initialize()
{
	bluetoothState = new InputState();
	bluetoothState->SetBluetoothState(new BluetoothState());


	thread t(&MeteoBluetoothPhoneV2::work, this);
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

int MeteoBluetoothPhoneV2::PushOutputMessage(OutputMessage * outputMessage)
{
	BluetoothMessage* bluetoothMessage = dynamic_cast<BluetoothMessage*>(outputMessage);

	if (bluetoothMessage != nullptr) {
		outputMessages.push_back(bluetoothMessage);

		return 0;

	}

	return -1;
}

int MeteoBluetoothPhoneV2::work()
{


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
	LOG(LogLevel::Debug) << "MeteoPacketConverterV1::writeBluetooth() : start writing.";
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

int MeteoBluetoothPhoneV2::handleNewPacket(char * packet, int length)
{

	LOG(LogLevel::Debug) << "MeteoPacketConverterV1::handleNewPacket() : length [" << length << "].";

	PacketType packetType = packetConverter->CheckPacketType(packet, length);

	/* 讀取韌體版號 */
	if (packetType == PacketType::ReadFirmwareVersion) {
		// TODO: 做一個firmware version專用的bt message class
		//MeteoBluetoothMessage* returnMessage = new MeteoContextBluetoothMessage(MeteoCommand::ReturnFirmwareVersion);
		//outputMessages.push_back(returnMessage);
	}
	else if (packetType == PacketType::Json) {

		BluetoothMessage* btMessage = packetConverter->ConvertToBluetoothMessage(packet, length);

		if (btMessage == nullptr) {
			LOG(LogLevel::Error) << "MeteoBluetoothPhoneV1::handleNewPacket() : convert to bt command failed.";
		}

		if (btMessage != nullptr)
			pushBluetoothState(btMessage);
	}
	else if (packetType == PacketType::File) {

		BluetoothMessage* btMessage = packetConverter->ConvertToFile(packet, length);

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
