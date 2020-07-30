#include "MeteoBluetoothPhoneV1.h"

#include <thread>
#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>


using namespace std;
using namespace Desktop::Devices;


sdp_session_t * MeteoBluetoothPhoneV1::register_service(int port)
{
	uint32_t svc_uuid_int[] = { 0, 0, 0, 0xABCD };
	uint8_t rfcomm_port = 11;
	uint16_t rfcomm_channel = port; //0x1;
	const char *service_name = "Meteo Keyboard";
	const char *service_dsc = "A music game keyboard";
	const char *service_prov = "Ultra Gbye";

	uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid, svc_class_uuid;
	sdp_list_t *l2cap_list = 0,
		*rfcomm_list = 0,
		*root_list = 0,
		*proto_list = 0,
		*access_proto_list = 0,
		*svc_class_list = 0,
		*profile_list = 0;
	sdp_data_t *channel = 0;
	sdp_profile_desc_t profile;
	sdp_record_t record = { 0 };
	sdp_session_t *session = 0;

	// PART ONE
	// set the general service ID
	sdp_uuid128_create(&svc_uuid, &svc_uuid_int);
	sdp_set_service_id(&record, svc_uuid);

	// set the service class
	sdp_uuid16_create(&svc_class_uuid, SERIAL_PORT_SVCLASS_ID);
	svc_class_list = sdp_list_append(0, &svc_class_uuid);
	sdp_set_service_classes(&record, svc_class_list);

	// set the Bluetooth profile information
	sdp_uuid16_create(&profile.uuid, SERIAL_PORT_PROFILE_ID);
	profile.version = 0x0100;
	profile_list = sdp_list_append(0, &profile);
	sdp_set_profile_descs(&record, profile_list);

	// make the service record publicly browsable
	sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
	root_list = sdp_list_append(0, &root_uuid);
	sdp_set_browse_groups(&record, root_list);

	// set l2cap information
	sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
	l2cap_list = sdp_list_append(0, &l2cap_uuid);
	proto_list = sdp_list_append(0, l2cap_list);

	// register the RFCOMM channel for RFCOMM sockets
	sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
	channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
	rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
	sdp_list_append(rfcomm_list, channel);
	sdp_list_append(proto_list, rfcomm_list);
	access_proto_list = sdp_list_append(0, proto_list);
	sdp_set_access_protos(&record, access_proto_list);

	// set the name, provider, and description
	sdp_set_info_attr(&record, service_name, service_prov, service_dsc);

	// PART TWO
	// connect to the local SDP server, register the service record, and
	// disconnect
	session = sdp_connect(BDADDR_ANY, BDADDR_LOCAL, 0);
	sdp_record_register(session, &record, 0);

	// cleanup
	sdp_data_free(channel);
	sdp_list_free(l2cap_list, 0);
	sdp_list_free(rfcomm_list, 0);
	sdp_list_free(root_list, 0);
	sdp_list_free(access_proto_list, 0);

	return session;
}

int MeteoBluetoothPhoneV1::init_server(int port)
{
	struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
	char buf[1024] = { 0 };
	int s, client, bytes_read;
	int opt = sizeof(rem_addr);

	session = register_service(port);

	// allocate socket
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	// bind socket to port 1 of the first available
	// local bluetooth adapter
	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = *BDADDR_ANY;
	loc_addr.rc_channel = (uint8_t)port;//1;
	bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

	// put socket into listening mode
	listen(s, 1);

	// accept one connection
	client = accept(s, (struct sockaddr *)&rem_addr, &opt);

	ba2str(&rem_addr.rc_bdaddr, buf);
	fprintf(stderr, "accepted connection from %s\n", buf);

	return client;
}

MeteoBluetoothPhoneV1::MeteoBluetoothPhoneV1(PacketConverter<MeteoCommand>* pConverter)
{
	packetConverter = pConverter;

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

		PacketType packetType = packetConverter->CheckPacketType(bufferIn, bytes_read);

		if (packetType == PacketType::Json) {

			BluetoothCommand* btCommand = packetConverter->ConvertToBluetoothCommand(bufferIn, bytes_read);

			// TODO:回傳收到

			if (btCommand != nullptr)
				pushBluetoothState(btCommand);
		}
		else if (packetType == PacketType::File) {

			packetConverter->ConvertToFile(bufferIn, bytes_read);

			// TODO:回傳收到
		}

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
			int bytes_write = packetConverter->ConvertToByteArray(outputMessages[i], bufferOut, 32768);

			write(client, bufferOut, bytes_write);

			memset(bufferOut, 0, sizeof(bufferOut));
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
