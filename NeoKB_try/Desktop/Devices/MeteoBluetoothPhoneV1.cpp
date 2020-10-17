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
	LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::readBluetooth() : start reading.";
	if(lastRunReceived)
		memset(bufferIn, 0, sizeof(bufferIn));

	int bytes_read = read(client, bufferIn, sizeof(bufferIn));
	if (bytes_read > 0) {
		lastRunReceived = true;

		if(bytes_read == 32768)
			LOG(LogLevel::Error) << "MeteoPacketConverterV1::readBluetooth() : buffer overflow.";

		char** packets = new char*[128];
		int* packetLengths = new int[128];

		LOG(LogLevel::Depricated) << "MeteoPacketConverterV1::readBluetooth() : spliting packet.";

		int packetCount = packetConverter->SplitPacket(bufferIn, bytes_read, packets, packetLengths);

		LOG(LogLevel::Debug) << "MeteoPacketConverterV1::readBluetooth() : packets [" << packetCount << "].";
		for (int i = 0; i < packetCount; i++) {

			PacketStatus packetStatus = packetConverter->CheckPacketStatus(packets[i], packetLengths[i]);

			if (packetStatus == PacketStatus::Fine) {
				handleNewPacket(packets[i], packetLengths[i]);
			}
			else {
				LOG(LogLevel::Error) << "MeteoPacketConverterV1::readBluetooth() : packet error with status [" << (int)packetStatus << "].";
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

			/* 如果是傳送檔案的話，要檢查有沒有傳送成功 */
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

			/* countdown要檢查重新輸出的檔案，Countdown完畢以後就開始檢查重新輸出 */
			if (outputMessagesToRewrite[i].first > 0) {
				MeteoOutputFileBluetoothCommand* outputMessageToRewrite = outputMessagesToRewrite[i].second;
				int tempOutputMessageToRewriteCountdown = outputMessagesToRewrite[i].first--;
				
				outputMessagesToRewrite.erase(outputMessagesToRewrite.begin() + i);
				outputMessagesToRewrite.insert(outputMessagesToRewrite.begin() + i, pair<int, MeteoOutputFileBluetoothCommand*>(tempOutputMessageToRewriteCountdown, outputMessageToRewrite));

			}
			/* 檢查是否需要重新輸出 */
			else {
				MeteoOutputFileBluetoothCommand* outputMessageToRewrite = dynamic_cast<MeteoOutputFileBluetoothCommand*>(outputMessagesToRewrite[i].second);

				if (outputMessageToRewrite->GetSegmentsToReOutput().size() > 0) {
					for (int j = 0; j < outputMessageToRewrite->GetSegmentsToReOutput().size(); j++) {

						int segmentNumber = outputMessageToRewrite->GetSegmentsToReOutput()[j];

						int bytes_write = packetConverter->ConvertToByteArray(outputMessageToRewrite, segmentNumber, bufferOut, 1024);

						write(client, bufferOut, bytes_write);

						memset(bufferOut, 0, sizeof(bufferOut));

					}

					/* 重新輸出完畢之後再丟回去重新countdown */
					outputMessagesToRewrite.erase(outputMessagesToRewrite.begin() + i);
					outputMessagesToRewrite.insert(outputMessagesToRewrite.begin() + i, pair<int, MeteoOutputFileBluetoothCommand*>(OutputMessageToRewriteCountdown, outputMessageToRewrite));

				}
				/* 對方全部接收到以後，就可以送FinishWrite指令給對方，確認還有沒有需要接收的片段 */
				else {

					// TODO: 寫送finish write封包
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

int MeteoBluetoothPhoneV1::pushBluetoothState(BluetoothCommand * btCommand)
{
	unique_lock<mutex> uLock(bluetoothStateMutex);

	bluetoothState->GetBluetoothState()->AddCommand(btCommand);

	return 0;
}

int MeteoBluetoothPhoneV1::handleNewPacket(char * packet, int length)
{

	LOG(LogLevel::Debug) << "MeteoPacketConverterV1::handleNewPacket() : length [" << length << "].";

	PacketType packetType = packetConverter->CheckPacketType(packet, length);

	/* 讀取韌體版號 */
	if (packetType == PacketType::ReadFirmwareVersion) {
		MeteoBluetoothCommand* returnCommand = new MeteoBluetoothCommand(MeteoCommand::ReturnFirmwareVersion);
		outputMessages.push_back(returnCommand);
	}
	else if (packetType == PacketType::Json) {

		BluetoothCommand* btCommand = packetConverter->ConvertToBluetoothCommand(packet, length);

		if (btCommand == nullptr) {
			LOG(LogLevel::Error) << "MeteoBluetoothPhoneV1::handleNewPacket() : convert to bt command failed.";
		}

		// TODO:檢查是不是傳送音色包，是的話要再converter裡面記錄音色包資料

		if (packetConverter->CheckIsWrtieFileFinishCommand(btCommand)) {
			BluetoothCommand* returnCommand = packetConverter->FinishWriteFile(btCommand);

			/* 發生錯誤 */
			if (returnCommand == nullptr)
				return 0;
			
			/* 確認整個檔案已經完成傳輸，讓sm manager把新的sm檔寫入sm info裡面 */
			if (dynamic_cast<MeteoBluetoothCommand*>(returnCommand)->GetCommand() == MeteoCommand::AckFinishWriteSheetmusic)
				onWriteSmFileSuccess.Trigger(returnCommand->GetContext()["FileName"].get<string>());
				
			/* 把回傳的訊息丟到手機 */
			outputMessages.push_back(dynamic_cast<MeteoBluetoothCommand*>(returnCommand));
			
		}
		// TODO:回傳收到，有時是這邊回傳，有時是其他物件回傳

		if (btCommand != nullptr)
			pushBluetoothState(btCommand);
	}
	else if (packetType == PacketType::File) {

		BluetoothCommand* ack = packetConverter->ConvertToFile(packet, length);

		if (ack != nullptr) 
			outputMessages.push_back(dynamic_cast<MeteoBluetoothCommand*>(ack));
		// TODO:回傳收到
	}
	else if (packetType == PacketType::AckFile) {

		BluetoothCommand* btCommand = packetConverter->ConvertToBluetoothCommand(packet, length);

		MeteoAckFileBluetoothCommand* ack = dynamic_cast<MeteoAckFileBluetoothCommand*>(btCommand);

		if (ack == nullptr) {
			LOG(LogLevel::Error) << "MeteoBluetoothPhoneV1::handleNewPacket() : ack command [" << (int)dynamic_cast<MeteoBluetoothCommand*>(btCommand)->GetCommand() << "] cannot cast to MeteoAckFileBluetoothCommand.";
			return -1;
		}


		for (int i = 0; i < outputMessagesToRewrite.size(); i++) {
			if (outputMessagesToRewrite[i].second->GetFileName() == ack->GetFileName()) {

				outputMessagesToRewrite[i].second->DeleteSegmentToReOutput(ack->GetOrder());

			}
		}

	}
	else if (packetType == PacketType::None) {
		// 封包壞掉，直接丟掉，不用刪因為return以後外面會刪
		// packetConverter->CleanBuffer();
	}

	return 0;
}
