#include "MeteoBluetoothPhoneV1.h"

#include <thread>
#include "sdp.h"
#include <unistd.h>


using namespace std;
using namespace Desktop::Devices;





sdp_session_t * MeteoBluetoothPhoneV1::register_service(int port)
{
	return sdp_register_service(port);
}

int MeteoBluetoothPhoneV1::init_server(int port)
{
	return sdp_init_server(port);
}

MeteoBluetoothPhoneV1::MeteoBluetoothPhoneV1(PacketConverter<MeteoCommand>* pConverter)
{
	packetConverter = pConverter;

	return;

	client = init_server(0x1);

	isConnected = true;

	bluetoothState = new InputState();
	bluetoothState->SetBluetoothState(new BluetoothState());


	thread t(&MeteoBluetoothPhoneV1::work, this);
	t.detach();


}

InputState * MeteoBluetoothPhoneV1::GetBluetoothState()
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

int MeteoBluetoothPhoneV1::PushOutputMessage(OutputMessage * outputMessage)
{
	BluetoothMessage* bluetoothMessage = dynamic_cast<BluetoothMessage*>(outputMessage);

	if (bluetoothMessage != nullptr) {
		BluetoothCommand* command = packetConverter->ConvertToBluetoothCommand(bluetoothMessage);
		if (dynamic_cast<MeteoBluetoothCommand*>(command)) {
			outputMessages.push_back(dynamic_cast<MeteoBluetoothCommand*>(command));
			
			return 0;
		}
	}


	return -1;
}

int MeteoBluetoothPhoneV1::work()
{
	while (!exitRequested) {
		readBluetooth();
		writeBluetooth();

		if(!lastRunReceived && !lastRunSended)
			this_thread::sleep_for(chrono::milliseconds(10));

	}
	return 0;
}

int MeteoBluetoothPhoneV1::readBluetooth()
{
	if(lastRunReceived)
		memset(bufferIn, 0, sizeof(bufferIn));

	int bytes_read = read(client, bufferIn, sizeof(bufferIn));
	if (bytes_read > 0) {
		lastRunReceived = true;

		char** packets = nullptr;
		int* packetLengths = nullptr;

		int packetCount = packetConverter->SplitPacket(bufferIn, bytes_read, packets, packetLengths);

		for (int i = 0; i < packetCount; i++) {

			PacketStatus packetStatus = packetConverter->CheckPacketStatus(packets[i], packetLengths[i]);

			if (packetStatus == PacketStatus::Fine) {
				handleNewPacket(packets[i], packetLengths[i]);
			}
			else {
				// �B�z���~���ʥ]
			}
		}

		// delete packet
		// delete packetLengths

	}
	else
		lastRunReceived = false;



	return 0;
}

int MeteoBluetoothPhoneV1::writeBluetooth()
{
	if (lastRunSended)
		memset(bufferOut, 0, sizeof(bufferOut));

	if (outputMessages.size() > 0) {

		for (int i = 0; i < outputMessages.size(); i++) {

			int packetCount = packetConverter->GetCountOfPacket(outputMessages[i]);

			for (int j = 0; j < packetCount; j++) {

				int bytes_write = packetConverter->ConvertToByteArray(outputMessages[i], j, bufferOut, 1024);

				write(client, bufferOut, bytes_write);

				memset(bufferOut, 0, sizeof(bufferOut));
			}
		}

		lastRunSended = true;
	}
	else
		lastRunSended = false;
	

	return 0;
}

int MeteoBluetoothPhoneV1::pushBluetoothState(BluetoothCommand * btCommand)
{
	unique_lock<mutex> uLock(bluetoothStateMutex);

	bluetoothState->GetBluetoothState()->AddCommand(btCommand);

	return 0;
}

int MeteoBluetoothPhoneV1::handleNewPacket(char * packet, int length)
{

	PacketType packetType = packetConverter->CheckPacketType(packet, length);

	if (packetType == PacketType::Json) {

		BluetoothCommand* btCommand = packetConverter->ConvertToBluetoothCommand(packet, length);

		// TODO:�ˬd�O���O�ǰe����]�A�O���ܭn�Aconverter�̭��O������]���

		if (packetConverter->CheckIsWrtieFileFinishCommand(btCommand)) {
			BluetoothCommand* returnCommand = packetConverter->FinishWriteFile(btCommand);
			outputMessages.push_back(dynamic_cast<MeteoBluetoothCommand*>(returnCommand));
			// ��sm manager��s��sm�ɼg�Jsm info�̭�
			onWriteSmFileSuccess.Trigger(returnCommand->GetContext()["FileName"].get<string>());
		}
		// TODO:�^�Ǧ���A���ɬO�o��^�ǡA���ɬO��L����^��

		if (btCommand != nullptr)
			pushBluetoothState(btCommand);
	}
	else if (packetType == PacketType::File) {

		packetConverter->ConvertToFile(packet, length);

		// TODO:�^�Ǧ���
	}
	else if (packetType == PacketType::None) {
		// �ʥ]�a���A�����ᱼ
		// packetConverter->CleanBuffer();
	}

	return 0;
}
