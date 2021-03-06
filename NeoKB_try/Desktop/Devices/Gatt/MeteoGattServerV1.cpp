#include "MeteoGattServerV1.h"

#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include "../../../Util/Log.h"
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <getopt.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include "../../../Util/CommandPasser.h"
#include "../../../Util/StringSplitter.h"
#include "MeteoGattClientV1.h"
#include <map>
#include "../../../Util/FileContentGetter.h"



// these are pulled directly from the BlueZ source tree
extern "C"
{
#include <src/shared/mainloop.h>
}

using namespace Desktop::Devices::Gatt;
using namespace Util;


void MeteoGattServerV1::EnqueueAsyncMessage(const char * buff, int n)
{
	lock_guard<mutex> guard(m_mutex);
	if (m_client)
		m_client->EnqueueForSend(buff, n);


}

void MeteoGattServerV1::OnIncomingMessage(const char * buff, int n)
{
	if (!buff || strlen(buff) == 0)
		return;

	std::lock_guard<std::mutex> guard(m_mutex);


	char* message = new char[n];

	memcpy(message, buff, n);

	m_incoming_queue.push(pair<char*, int>(message, n));


}

GattServer::GattListener * MeteoGattServerV1::createListener()
{
	GattListener* listener = new MeteoGattListenerV1();

	vector<string> config;
	config.push_back("MeteoPiano48");
	config.push_back("0");
	listener->Init(config);

	return listener;
}

GattClient * MeteoGattServerV1::accept()
{
	map<string, function<string()>> deviceInfoGetter;

	deviceInfoGetter["GetSystemId"]			= bind(&MeteoGattServerV1::getSystemId, this);
	deviceInfoGetter["GetModelNumber"]		= bind(&MeteoGattServerV1::getModelNumber, this);
	deviceInfoGetter["GetSerialNumber"]		= bind(&MeteoGattServerV1::getSerialNumber, this);
	deviceInfoGetter["GetFirmwareRevision"] = bind(&MeteoGattServerV1::getFirmwareRevision, this);
	deviceInfoGetter["GetHardwareRevision"] = bind(&MeteoGattServerV1::getHardwareRevision, this);
	deviceInfoGetter["GetSoftwareRevision"] = bind(&MeteoGattServerV1::getSoftwareRevision, this);
	deviceInfoGetter["GetManufacturerName"] = bind(&MeteoGattServerV1::getManufacturerName, this);

	return gattListener->Accept(deviceInfoGetter);
}

string MeteoGattServerV1::getSystemId()
{
	return FileContentGetter::GetContentFromFile("/etc/machine-id");;
}

string MeteoGattServerV1::getModelNumber()
{
	string s = FileContentGetter::GetContentFromFile("/proc/device-tree/model");
	size_t n = s.size();
	if (s[n - 1] == '\0')
		s = s.substr(0, (n - 1));
	return s;
}

string MeteoGattServerV1::getSerialNumber()
{
	string s = FileContentGetter::GetFileContentVariable("/proc/cpuinfo", "Serial");
	LOG(LogLevel::Info) << "serial:" << s;
	return s;
}

string MeteoGattServerV1::getFirmwareRevision()
{
	// get version from command "uname -a"
	string full = CommandPasser::PassCommand("uname -a");
	size_t index = full.find(" SMP");
	if (index != string::npos)
	{
		return full.substr(0, index);
	}
	return full;
}

string MeteoGattServerV1::getHardwareRevision()
{
	return FileContentGetter::GetFileContentVariable("/proc/cpuinfo", "Revision");
}

string MeteoGattServerV1::getSoftwareRevision()
{
	// TODO: 放入軟體版本號
	return string("0");
}

string MeteoGattServerV1::getManufacturerName()
{
	string rCode = getHardwareRevision();
	vector<string> deviceInfo;

	if (rCode == "9020e0") {
		deviceInfo.push_back("9020e0");
		deviceInfo.push_back("3A+");
		deviceInfo.push_back("1.0");
		deviceInfo.push_back("512 MB");
		deviceInfo.push_back("Sony UK");
	}
	else if (rCode == "a020d3") {
		deviceInfo.push_back("a020d3");
		deviceInfo.push_back("3B+");
		deviceInfo.push_back("1.3");
		deviceInfo.push_back("1 GB");
		deviceInfo.push_back("Sony UK");
	}

	/*
	900021,A+,1.1,512 MB,Sony UK
	900032,B+,1.2,512 MB,Sony UK
	900092,Zero,1.2,512 MB,Sony UK
	920092,Zero,1.2,512 MB,Embest
	900093,Zero,1.3,512 MB,Sony UK
	9000c1,Zero W,1.1,512 MB,Sony UK
	920093,Zero,1.3,512 MB,Embest
	a01040,2B,1.0,1 GB,Sony UK
	a01041,2B,1.1,1 GB,Sony UK
	a02082,3B,1.2,1 GB,Sony UK
	a020a0,CM3,1.0,1 GB,Sony UK
	a21041,2B,1.1,1 GB,Embest
	a22042,2B (with BCM2837),1.2,1 GB,Embest
	a22082,3B,1.2,1 GB,Embest
	a32082,3B,1.2,1 GB,Sony Japan
	a52082,3B,1.2,1 GB,Stadium
	a020d3,3B+,1.3,1 GB,Sony UK
	9020e0,3A+,1.0,512 MB,Sony UK
	*/


	if (deviceInfo.size())
	{
		//return deviceInfo[4];
	}
	return string("Meteo TW");
}

int MeteoGattServerV1::MeteoGattListenerV1::Init(vector<string> config)
{


	m_listen_fd = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	if (m_listen_fd < 0)
		throw_errno(errno, "failed to create bluetooth socket");

	bdaddr_t src_addr = { 0 }; // BDADDR_ANY

	sockaddr_l2 srcaddr;
	memset(&srcaddr, 0, sizeof(srcaddr));
	srcaddr.l2_family = AF_BLUETOOTH;
	srcaddr.l2_cid = htobs(4);
	srcaddr.l2_bdaddr_type = BDADDR_LE_PUBLIC;
	bacpy(&srcaddr.l2_bdaddr, &src_addr);

	int ret = bind(m_listen_fd, reinterpret_cast<sockaddr *>(&srcaddr), sizeof(srcaddr));
	if (ret < 0)
		throw_errno(errno, "failed to bind bluetooth socket");

	bt_security btsec = { 0, 0 };
	btsec.level = BT_SECURITY_LOW;
	ret = setsockopt(m_listen_fd, SOL_BLUETOOTH, BT_SECURITY, &btsec, sizeof(btsec));
	if (ret < 0)
		throw_errno(errno, "failed to set security on bluetooth socket");

	ret = listen(m_listen_fd, 2);
	if (ret < 0)
		throw_errno(errno, "failed to listen on bluetooth socket");

	string name = config.size() > 0 ? config[0] : "Meteo";
	string id = config.size() > 1 ? config[1] : "0"; // 之後再補

	

	startBeacon(name, stoi(id));


	return 0;
}

GattClient * MeteoGattServerV1::MeteoGattListenerV1::Accept(map<string, function<string()>> deviceInfoGetter)
{
	mainloop_init();

	sockaddr_l2 peer_addr;
	memset(&peer_addr, 0, sizeof(peer_addr));

	LOG(LogLevel::Info) << "waiting for incoming BLE connections";

	socklen_t n = sizeof(peer_addr);
	int soc = ::accept(m_listen_fd, reinterpret_cast<sockaddr *>(&peer_addr), &n);
	if (soc < 0)
		throw_errno(errno, "failed to accept incoming connection on bluetooth socket");

	char remote_address[64] = { 0 };
	ba2str(&peer_addr.l2_bdaddr, remote_address);
	LOG(LogLevel::Info) << "accepted remote connection from:" << remote_address;

	auto client = new MeteoGattClientV1(soc);
	client->Init(deviceInfoGetter);
	return client;
}

int MeteoGattServerV1::MeteoGattListenerV1::startBeacon(string name, int id)
{
	LOG(LogLevel::Info) << "int MeteoGattListenerV1::startBeacon() : [" << name << "] on hci" << id;

	// updateDeviceName(s.c_str());
	struct hci_dev_info deviceInfo;
	uint8_t instanceId[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

	int ctl = 0;
	if ((ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI)) < 0)
	{
		LOG(LogLevel::Error) << "int MeteoGattListenerV1::startBeacon() : Can't open HCI socket.";
	}

	deviceInfo.dev_id = id;

	if (ioctl(ctl, HCIGETDEVINFO, (void*)&deviceInfo))
	{
		LOG(LogLevel::Error) << "Can't get device info";
	}

	bdaddr_t any = { 0 }; // BDADDR_ANY
	if (hci_test_bit(HCI_RAW, &deviceInfo.flags) && !bacmp(&deviceInfo.bdaddr, &any))
	{
		int dd = hci_open_dev(deviceInfo.dev_id);
		hci_read_bd_addr(dd, &deviceInfo.bdaddr, 1000);
		hci_close_dev(dd);
	}
	else
	{
		char addr[18];
		ba2str(&deviceInfo.bdaddr, addr);
		LOG(LogLevel::Info) << "bluetooth mac: " << addr;

		for (int i = 0; i < 6; ++i)
			instanceId[i] = deviceInfo.bdaddr.b[i];
	}

	cmdDown(ctl, deviceInfo.dev_id);
	cmdUp(ctl, deviceInfo.dev_id);
	cmdNoleadv(deviceInfo.dev_id);

	// TODO: hardcoded for now to
	// [antman]: sudo hciconfig hci0 class 3a0430
	// [antman]: sudo hciconfig hci0 class
	// hci0:   Type: Primary  Bus: UART
	//    BD Address: B8:27:EB:A0:DA:2C  ACL MTU: 1021:8  SCO MTU: 64:1
	//    Class: 0x3a0430
	//    Service Classes: Networking, Capturing, Object Transfer, Audio
	//    Device Class: Audio/Video, Video Camera
	system("hciconfig hci0 class 3a0430");

	char buff[128];
	snprintf(buff, sizeof(buff), "0x08 0x0008 "
		"1f 02 01 06 03 03 aa fe 17 16 aa fe 00 e7 36 c8 80 7b f4 60 cb 41 d1 45 %02x %02x %02x %02x %02x %02x 00 00",
		instanceId[5],
		instanceId[4],
		instanceId[3],
		instanceId[2],
		instanceId[1],
		instanceId[0]);

	hcitoolCmd(deviceInfo.dev_id, parseArgs(buff));

	system("hcitool cmd 0x08 0x0006 A0 00 A0 00 00 00 00 00 00 00 00 00 00 07 00");

#if 0
	std::string startUpCmd02(appSettings_get_ble_value("ble_init_cmd02"));
	hcitoolCmd(di.dev_id, parseArgs(startUpCmd02));
#endif

	cmdLeadv(deviceInfo.dev_id);
	cmdName(deviceInfo.dev_id, name.c_str());

	return 0;
}

void MeteoGattServerV1::MeteoGattListenerV1::cmdDown(int ctl, int hdev)
{
	if (ioctl(ctl, HCIDEVDOWN, hdev) < 0)
	{
		LOG(LogLevel::Error) << "Can't down device hci" << hdev << ": " << strerror(errno) << " (" << errno << ")";
	}
}

void MeteoGattServerV1::MeteoGattListenerV1::cmdUp(int ctl, int hdev)
{
	if (ioctl(ctl, HCIDEVUP, hdev) < 0)
	{
		if (errno == EALREADY)
			return;
		LOG(LogLevel::Error) << "Can't init device hci" << hdev << ": " << strerror(errno) << " (" << errno << ")";
	}

}

void MeteoGattServerV1::MeteoGattListenerV1::cmdNoleadv(int hdev)
{
	hci_request req;
	le_set_advertise_enable_cp advertise_cp;

	uint8_t status = 0;
	int dd = 0;
	int ret = 0;

	if (hdev < 0)
		hdev = hci_get_route(NULL);

	dd = hci_open_dev(hdev);
	if (dd < 0)
	{
		LOG(LogLevel::Error) << "Could not open device " << hdev;
	}

	memset(&advertise_cp, 0, sizeof(advertise_cp));
	advertise_cp.enable = 0x00;

	memset(&req, 0, sizeof(req));
	req.ogf = OGF_LE_CTL;
	req.ocf = OCF_LE_SET_ADVERTISE_ENABLE;
	req.cparam = &advertise_cp;
	req.clen = LE_SET_ADVERTISE_ENABLE_CP_SIZE;
	req.rparam = &status;
	req.rlen = 1;

	ret = hci_send_req(dd, &req, 1000);

	hci_close_dev(dd);

	if (ret < 0)
	{
		LOG(LogLevel::Error) << "Can't set advertise mode on hci" << hdev << ": " << strerror(errno) << " (" << errno << ")";
	}

	// TODO: what exactly is "status"
	if (status)
	{
		LOG(LogLevel::Info) << "Disabling LE advertise on hci" << hdev << " returned error status:" << status << ".";
	}
}

void MeteoGattServerV1::MeteoGattListenerV1::cmdLeadv(int hdev)
{
	struct hci_request rq;
	le_set_advertise_enable_cp advertise_cp;
	le_set_advertising_parameters_cp adv_params_cp;
	uint8_t status;
	int dd, ret;

	if (hdev < 0)
		hdev = hci_get_route(NULL);

	dd = hci_open_dev(hdev);
	if (dd < 0)
	{
		LOG(LogLevel::Error) << "Could not open device " << hdev;
	}

	memset(&adv_params_cp, 0, sizeof(adv_params_cp));
	adv_params_cp.min_interval = htobs(0x0800);
	adv_params_cp.max_interval = htobs(0x0800);
	adv_params_cp.chan_map = 7;

	memset(&rq, 0, sizeof(rq));
	rq.ogf = OGF_LE_CTL;
	rq.ocf = OCF_LE_SET_ADVERTISING_PARAMETERS;
	rq.cparam = &adv_params_cp;
	rq.clen = LE_SET_ADVERTISING_PARAMETERS_CP_SIZE;
	rq.rparam = &status;
	rq.rlen = 1;

	ret = hci_send_req(dd, &rq, 1000);
	if (ret >= 0)
	{
		memset(&advertise_cp, 0, sizeof(advertise_cp));
		advertise_cp.enable = 0x01;

		memset(&rq, 0, sizeof(rq));
		rq.ogf = OGF_LE_CTL;
		rq.ocf = OCF_LE_SET_ADVERTISE_ENABLE;
		rq.cparam = &advertise_cp;
		rq.clen = LE_SET_ADVERTISE_ENABLE_CP_SIZE;
		rq.rparam = &status;
		rq.rlen = 1;
		ret = hci_send_req(dd, &rq, 1000);
	}

	hci_close_dev(dd);

	if (ret < 0)
	{
		LOG(LogLevel::Error) << "Can't set advertise mode on hci" << hdev << ": " << strerror(errno) << " (" << errno << ")";
	}

	if (status)
	{
		LOG(LogLevel::Warning) << "Enabling LE advertise mode on hci" << hdev << " returned error status: " << status;
	}
}

void MeteoGattServerV1::MeteoGattListenerV1::hex_dump(int width, unsigned char * buf, int len)
{
	char const* pref = "  ";
	register int i, n;
	for (i = 0, n = 1; i < len; i++, n++)
	{
		if (n == 1)
			printf("%s", pref);
		printf("%2.2X ", buf[i]);
		if (n == width)
		{
			printf("\n");
			n = 0;
		}
	}
	if (i && n != 1)
		printf("\n");
}

void MeteoGattServerV1::MeteoGattListenerV1::cmdName(int hdev, char const * deviceName)
{
	int dd = hci_open_dev(hdev);
	if (dd < 0)
	{
		LOG(LogLevel::Error) << "Can't open device hci" << hdev << ": " << strerror(errno) << " (" << errno << ")";
	}

	if (hci_write_local_name(dd, deviceName, 2000) < 0)
	{
		LOG(LogLevel::Error) << "Can't change local name hci" << hdev << ": " << strerror(errno) << " (" << errno << ")";
	}

	char name[249];
	if (hci_read_local_name(dd, sizeof(name), name, 1000) < 0)
	{
		LOG(LogLevel::Error) << "Can't read local name hci" << hdev << ": " << strerror(errno) << " (" << errno << ")";
	}

	for (int i = 0; i < 248 && name[i]; i++)
	{
		if ((unsigned char)name[i] < 32 || name[i] == 127)
			name[i] = '.';
	}
	name[248] = '\0';

	LOG(LogLevel::Info) << "Device Name: " << name;
	hci_close_dev(dd);
}

string MeteoGattServerV1::MeteoGattListenerV1::getDeviceNameFromFile()
{
	string str_buffer = CommandPasser::PassCommand("cat /etc/machine-info");
	if (str_buffer.find("PRETTY_HOSTNAME") != string::npos)
	{
		str_buffer = str_buffer.substr(0, str_buffer.length() - 1);
		return StringSplitter::Split(str_buffer, "=")[1];
	}
	return string();
}

void MeteoGattServerV1::MeteoGattListenerV1::updateDeviceName(string const & name)
{
	// device name is same as new name, skip it
	if (!name.compare(getDeviceNameFromFile()))
	{
		return;
	}
	char cmd_buffer[256];
	sprintf(cmd_buffer, "sh -c \"echo 'PRETTY_HOSTNAME=%s' > /etc/machine-info\"", name.c_str());
	CommandPasser::PassCommand(cmd_buffer);
	CommandPasser::PassCommand("service bluetooth restart");
	LOG(LogLevel::Debug) << "device set new name = " << getDeviceNameFromFile().c_str();
}

void MeteoGattServerV1::MeteoGattListenerV1::hcitoolCmd(int dev_id, vector<string> const & args)
{
	unsigned char buf[HCI_MAX_EVENT_SIZE], *ptr = buf;
	struct hci_filter flt;
	hci_event_hdr* hdr;
	int i, dd;
	int len;
	uint16_t ocf;
	uint8_t ogf;

	if (dev_id < 0)
	{
		dev_id = hci_get_route(NULL);
	}

	errno = 0;
	ogf = strtol(args[0].c_str(), NULL, 16);
	ocf = strtol(args[1].c_str(), NULL, 16);
	if (errno == ERANGE || (ogf > 0x3f) || (ocf > 0x3ff))
	{
		LOG(LogLevel::Error) << "ogf must be in range (0x3f,0x3ff)";
	}

	for (i = 2, len = 0; i < (int)args.size() && len < (int) sizeof(buf); i++, len++)
		*ptr++ = (uint8_t)strtol(args[i].c_str(), NULL, 16);

	dd = hci_open_dev(dev_id);
	if (dd < 0)
	{
		LOG(LogLevel::Error) << "Device open failed";
	}

	/* Setup filter */
	hci_filter_clear(&flt);
	hci_filter_set_ptype(HCI_EVENT_PKT, &flt);
	hci_filter_all_events(&flt);
	if (setsockopt(dd, SOL_HCI, HCI_FILTER, &flt, sizeof(flt)) < 0)
	{
		LOG(LogLevel::Error) << "HCI filter setup failed";
	}

	LOG(LogLevel::Info) << "< HCI Command: ogf " << hex << ogf << ", ocf " << hex << ocf << ", len " << dec << len;
	hex_dump(20, buf, len);
	fflush(stdout);

	if (hci_send_cmd(dd, ogf, ocf, len, buf) < 0)
	{
		LOG(LogLevel::Error) << "Send failed";
	}

	len = read(dd, buf, sizeof(buf));
	if (len < 0)
	{
		LOG(LogLevel::Error) << "Read failed";
	}

	hdr = static_cast<hci_event_hdr*>((void*)(buf + 1));
	ptr = buf + (1 + HCI_EVENT_HDR_SIZE);
	len -= (1 + HCI_EVENT_HDR_SIZE);

	LOG(LogLevel::Info) << "> HCI Event: " << hex << hdr->evt << " plen " << dec << hdr->plen;
	hex_dump(20, ptr, len);
	fflush(stdout);

	hci_close_dev(dd);
}

vector<string> MeteoGattServerV1::MeteoGattListenerV1::parseArgs(string str)
{
	string delimiter = " ";

	size_t pos = 0;
	string token;
	vector <string> args;

	while ((pos = str.find(delimiter)) != string::npos)
	{
		token = str.substr(0, pos);
		args.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	args.push_back(str);
	return args;
}
