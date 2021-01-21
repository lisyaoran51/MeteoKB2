#include "MeteoBluetoothPhoneV1.h"

#include <thread>
#include "sdp.h"
#include <unistd.h>
#include "../../Games/Output/Bluetooths/Commands/MeteoOutputFileBluetoothCommand.h"
#include "../../Games/Input/Commands/MeteoAckFileBluetoothCommand.h"


using namespace std;
using namespace Desktop::Devices;
using namespace Games::Output::Bluetooths::Commands;
using namespace Games::Input::Commands;





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

}

int MeteoBluetoothPhoneV1::Initialize()
{
	bluetoothState = new InputState();
	bluetoothState->SetBluetoothState(new BluetoothState());


	thread t(&MeteoBluetoothPhoneV1::work, this);
	t.detach();
	return 0;
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


	client = init_server(0x1);

	isConnected = true;

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
	LOG(LogLevel::Depricated) << "MeteoBluetoothPhoneV1::readBluetooth() : start reading.";
	if(lastRunReceived)
		memset(bufferIn, 0, sizeof(bufferIn));

	int bytes_read = read(client, bufferIn, sizeof(bufferIn));
	if (bytes_read > 0) {
		lastRunReceived = true;

		if(bytes_read == 32768)
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
				// �B�z���~���ʥ]
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

int MeteoBluetoothPhoneV1::writeBluetooth()
{
	LOG(LogLevel::Debug) << "MeteoPacketConverterV1::writeBluetooth() : start writing.";
	if (lastRunSended)
		memset(bufferOut, 0, sizeof(bufferOut));

	if (outputMessages.size() > 0 || outputMessagesToRewrite.size() > 0) {

		for (int i = 0; i < outputMessages.size(); i++) {

			int packetCount = packetConverter->GetCountOfPacket(outputMessages[i]);

			for (int j = 0; j < packetCount; j++) {

				int bytes_write = packetConverter->ConvertToByteArray(outputMessages[i], j, bufferOut, 1024);

				write(client, bufferOut, bytes_write);

				memset(bufferOut, 0, sizeof(bufferOut));
			}

			/* �p�G�O�ǰe�ɮת��ܡA�n�ˬd���S���ǰe���\ */
			if (packetConverter->CheckCommandType(outputMessages[i]) == PacketType::File) {
				outputMessagesToRewrite.push_back(pair<int, MeteoOutputFileBluetoothCommand*>(OutputMessageToRewriteCountdown, dynamic_cast<MeteoOutputFileBluetoothCommand*>(outputMessages[i])));
			}
			else {
				delete outputMessages[i];
			}

			outputMessages.erase(outputMessages.begin() + i);
			i--;
		}

		for (int i = 0; i < outputMessagesToRewrite.size(); i++) {

			/* countdown�n�ˬd���s��X���ɮסACountdown�����H��N�}�l�ˬd���s��X */
			if (outputMessagesToRewrite[i].first > 0) {
				MeteoOutputFileBluetoothCommand* outputMessageToRewrite = outputMessagesToRewrite[i].second;
				int tempOutputMessageToRewriteCountdown = outputMessagesToRewrite[i].first--;
				
				outputMessagesToRewrite.erase(outputMessagesToRewrite.begin() + i);
				outputMessagesToRewrite.insert(outputMessagesToRewrite.begin() + i, pair<int, MeteoOutputFileBluetoothCommand*>(tempOutputMessageToRewriteCountdown, outputMessageToRewrite));

			}
			/* �ˬd�O�_�ݭn���s��X */
			else {
				MeteoOutputFileBluetoothCommand* outputMessageToRewrite = dynamic_cast<MeteoOutputFileBluetoothCommand*>(outputMessagesToRewrite[i].second);

				if (outputMessageToRewrite->GetSegmentsToReOutput().size() > 0) {
					for (int j = 0; j < outputMessageToRewrite->GetSegmentsToReOutput().size(); j++) {

						int segmentNumber = outputMessageToRewrite->GetSegmentsToReOutput()[j];

						int bytes_write = packetConverter->ConvertToByteArray(outputMessageToRewrite, segmentNumber, bufferOut, 1024);

						write(client, bufferOut, bytes_write);

						memset(bufferOut, 0, sizeof(bufferOut));

					}

					/* ���s��X��������A��^�h���scountdown */
					outputMessagesToRewrite.erase(outputMessagesToRewrite.begin() + i);
					outputMessagesToRewrite.insert(outputMessagesToRewrite.begin() + i, pair<int, MeteoOutputFileBluetoothCommand*>(OutputMessageToRewriteCountdown, outputMessageToRewrite));

				}
				/* ������������H��A�N�i�H�eFinishWrite���O�����A�T�{�٦��S���ݭn���������q */
				else {

					// TODO: �g�efinish write�ʥ]
					MeteoBluetoothCommand* outputMessageToRewrite = outputMessagesToRewrite[i].second;
					outputMessagesToRewrite.erase(outputMessagesToRewrite.begin() + i);
					delete outputMessageToRewrite;

					i--;
				}
			}
		}
	
		lastRunSended = true;
	}
	else
		lastRunSended = false;
	

	return 0;
}

int MeteoBluetoothPhoneV1::pushBluetoothState(BluetoothMessage * btMessage)
{
	LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV1::pushBluetoothState() : got new bt command.";

	unique_lock<mutex> uLock(bluetoothStateMutex);

	bluetoothState->GetBluetoothState()->AddMessage(btMessage);

	return 0;
}

int MeteoBluetoothPhoneV1::handleNewPacket(char * packet, int length)
{

	LOG(LogLevel::Debug) << "MeteoPacketConverterV1::handleNewPacket() : length [" << length << "].";

	PacketType packetType = packetConverter->CheckPacketType(packet, length);

	/* Ū�����骩�� */
	if (packetType == PacketType::ReadFirmwareVersion) {
		MeteoBluetoothMessage* returnMessage = new MeteoBluetoothMessage(MeteoCommand::ReturnFirmwareVersion);
		outputMessages.push_back(returnMessage);
	}
	else if (packetType == PacketType::Json) {

		BluetoothMessage* btMessage = packetConverter->ConvertToBluetoothMessage(packet, length);

		if (btMessage == nullptr) {
			LOG(LogLevel::Error) << "MeteoBluetoothPhoneV1::handleNewPacket() : convert to bt command failed.";
		}

		// TODO:�ˬd�O���O�ǰe����]�A�O���ܭn�Aconverter�̭��O������]���

		if (packetConverter->CheckIsWrtieFileFinishCommand(btMessage)) {
			BluetoothMessage* returnMessage = packetConverter->FinishWriteFile(btMessage);

			/* �o�Ϳ��~ */
			if (returnMessage == nullptr)
				return 0;
			
			/* �T�{����ɮפw�g�����ǿ�A��sm manager��s��sm�ɼg�Jsm info�̭� */
			if (dynamic_cast<MeteoBluetoothMessage*>(returnMessage)->GetCommand() == MeteoCommand::AckFinishWriteSheetmusic)
				onWriteSmFileSuccess.Trigger(returnMessage->GetContext()["FileName"].get<string>());
				
			/* ��^�Ǫ��T������� */
			outputMessages.push_back(dynamic_cast<MeteoBluetoothMessage*>(returnMessage));
			
		}
		// TODO:�^�Ǧ���A���ɬO�o��^�ǡA���ɬO��L����^��

		if (btMessage != nullptr)
			pushBluetoothState(btMessage);
	}
	else if (packetType == PacketType::File) {

		BluetoothMessage* ack = packetConverter->ConvertToFile(packet, length);

		if (ack != nullptr) 
			outputMessages.push_back(dynamic_cast<MeteoBluetoothMessage*>(ack));
		// TODO:�^�Ǧ���
	}
	else if (packetType == PacketType::AckFile) {

		BluetoothMessage* btMessage = packetConverter->ConvertToBluetoothMessage(packet, length);

		BluetoothMessage* ack = dynamic_cast<BluetoothMessage*>(btMessage);

		if (ack == nullptr) {
			LOG(LogLevel::Error) << "MeteoBluetoothPhoneV1::handleNewPacket() : ack command [" << (int)dynamic_cast<MeteoBluetoothMessage*>(btMessage)->GetCommand() << "] cannot cast to MeteoAckFileBluetoothCommand.";
			return -1;
		}


		for (int i = 0; i < outputMessagesToRewrite.size(); i++) {
			if (outputMessagesToRewrite[i].second->GetFileName() == ack->GetFileName()) {

				outputMessagesToRewrite[i].second->DeleteSegmentToReOutput(ack->GetOrder());

			}
		}

	}
	else if (packetType == PacketType::None) {
		// �ʥ]�a���A�����ᱼ�A���ΧR�]��return�H��~���|�R
		// packetConverter->CleanBuffer();
		LOG(LogLevel::Debug) << "MeteoBluetoothPhoneV1::handleNewPacket() : got error packet.";
	}

	return 0;
}
