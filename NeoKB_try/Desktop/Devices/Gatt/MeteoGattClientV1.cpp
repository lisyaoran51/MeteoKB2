#include "MeteoGattClientV1.h"

#include "../../../Util/Log.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <exception>
#include <assert.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

#define UNUSED_PARAM(X) UNUSED_ ## X __attribute__((__unused__))

// these are pulled directly from the BlueZ source tree
extern "C"
{
#include <src/shared/mainloop.h>
}

using namespace Desktop::Devices::Gatt;
using namespace Util;

void DIS_writeCallback(gatt_db_attribute* UNUSED_PARAM(attr), int err, void* UNUSED_PARAM(argp))
{
	if (err)
	{
		LOG(LogLevel::Warning) << "error writing to DIS service in GATT db. " << err;
	}
}

void ATT_debugCallback(char const* str, void* UNUSED_PARAM(argp))
{
	if (!str)
		LOG(LogLevel::Debug) << "ATT: debug callback with no message";
	else
		LOG(LogLevel::Debug) << "ATT: " << str;
}

void GATT_debugCallback(char const* str, void* UNUSED_PARAM(argp))
{
	if (!str)
		LOG(LogLevel::Debug) << "GATT: debug callback with no message";
	else
		LOG(LogLevel::Debug) << "GATT: " << str;
}



/*------------------------------------------------已上市工具------------------------------------------------*/

void GattClient_onGapRead(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att, void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onGapRead(attr, id, offset, opcode, att);
}

void GattClient_onGapWrite(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t const* data, size_t len, uint8_t opcode, bt_att* att, void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onGapWrite(attr, id, offset, data, len, opcode, att);
}

void GattClient_onServiceChanged(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att, void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onServiceChanged(attr, id, offset, opcode, att);
}

void GattClient_onServiceChangedRead(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att, void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onServiceChangedRead(attr, id, offset, opcode, att);
}

void GattClient_onServiceChangedWrite(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t const* value, size_t len, uint8_t opcode, bt_att* att, void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onServiceChangedWrite(attr, id, offset, value, len, opcode, att);
}

void GattClient_onGapExtendedPropertiesRead(gatt_db_attribute *attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att, void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onGapExtendedPropertiesRead(attr, id, offset, opcode, att);
}

void GattClient_onClientDisconnected(int err, void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onClientDisconnected(err);
}

void GattClient_onEPollRead(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att, void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onEPollRead(attr, id, offset, opcode, att);
}

void GattClient_onDataChannelIn(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t const* data, size_t len, uint8_t opcode, bt_att* att, void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onDataChannelIn(attr, id, offset, data, len, opcode, att);
}

void GattClient_onDataChannelOut(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att, void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onDataChannelOut(attr, id, offset, opcode, att);
}

void GattClient_onTimeout(int UNUSED_PARAM(fd), void* argp)
{
	MeteoGattClientV1* clnt = reinterpret_cast<MeteoGattClientV1 *>(argp);
	clnt->onTimeout();
}


MeteoGattClientV1::MeteoGattClientV1(int fd) : 
	m_fd(-1),
	m_att(nullptr),
	m_db(nullptr),
	m_server(nullptr),
	m_mtu(256),
	m_outgoing_queue(kRecordDelimiter),
	m_incoming_buff(),
	m_data_channel(nullptr),
	m_blepoll(nullptr),
	m_service_change_enabled(nullptr),
	m_timeout_id(-1),
	m_mainloop_thread()
{
}

MeteoGattClientV1::~MeteoGattClientV1()
{
	if (m_fd != -1)
		close(m_fd);

	if (m_server)
		bt_gatt_server_unref(m_server);

	if (m_db)
		gatt_db_unref(m_db);
}

void MeteoGattClientV1::Init(std::map<std::string, std::function<std::string()>> deviceInfoGetter)
{
	m_att = bt_att_new(m_fd, 0);
	if (!m_att)
	{
		LOG(LogLevel::Error) << "failed to create new att:" << errno;
	}

	bt_att_set_close_on_unref(m_att, true);
	bt_att_register_disconnect(m_att, &GattClient_onClientDisconnected, this, nullptr);
	m_db = gatt_db_new();
	if (!m_db)
	{
		LOG(LogLevel::Error) << "failed to create gatt database";
	}

	m_server = bt_gatt_server_new(m_db, m_att, m_mtu, 0);
	if (!m_server)
	{
		LOG(LogLevel::Error) << "failed to create gatt server";
	}

	if (true)
	{
		bt_att_set_debug(m_att, ATT_debugCallback, this, nullptr);
		bt_gatt_server_set_debug(m_server, GATT_debugCallback, this, nullptr);
	}

	m_timeout_id = mainloop_add_timeout(1000, &GattClient_onTimeout, this, nullptr);

	buildService(deviceInfoGetter);

}

void MeteoGattClientV1::EnqueueForSend(char const * buff, int n)
{
	if (!buff)
	{
		//XLOG_WARN("trying to enqueue null buffer");
		return;
	}

	if (n <= 0)
	{
		//XLOG_WARN("invalid buffer length:%d", n);
		return;
	}

	m_outgoing_queue.PutLine(buff, n);
}

void MeteoGattClientV1::Run()
{
	m_mainloop_thread = std::this_thread::get_id();

	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);

	mainloop_run();
}

void MeteoGattClientV1::SetDataHandler(std::function<void(char const*, int)> dHandler)
{
	m_data_handler = dHandler;
}

int MeteoGattClientV1::buildService(std::map<std::string, std::function<std::string()>> deviceInfoGetter)
{
	buildGapService();
	buildGattService();
	buildDeviceInfoService(deviceInfoGetter);
	buildMeteoService();
	return 0;
}

int MeteoGattClientV1::buildGapService()
{
	bt_uuid_t uuid;
	bt_uuid16_create(&uuid, kUuidGap);

	gatt_db_attribute* service = gatt_db_add_service(m_db, &uuid, true, 6);

	// device name
	bt_uuid16_create(&uuid, GATT_CHARAC_DEVICE_NAME);
	gatt_db_service_add_characteristic(service, &uuid, BT_ATT_PERM_READ | BT_ATT_PERM_WRITE,
		BT_GATT_CHRC_PROP_READ | BT_GATT_CHRC_PROP_EXT_PROP,
		&GattClient_onGapRead, &GattClient_onGapWrite, this);

	bt_uuid16_create(&uuid, GATT_CHARAC_EXT_PROPER_UUID);
	gatt_db_service_add_descriptor(service, &uuid, BT_ATT_PERM_READ,
		&GattClient_onGapExtendedPropertiesRead, nullptr, this);

	// appearance
	bt_uuid16_create(&uuid, GATT_CHARAC_APPEARANCE);
	gatt_db_attribute* attr = gatt_db_service_add_characteristic(service, &uuid, BT_ATT_PERM_READ,
		BT_GATT_CHRC_PROP_READ, nullptr, nullptr, this);

	uint16_t appearance{ 0 };
	bt_put_le16(128, &appearance);
	gatt_db_attribute_write(attr, 0, reinterpret_cast<uint8_t const *>(&appearance),
		sizeof(appearance), BT_ATT_OP_WRITE_REQ, nullptr, &DIS_writeCallback, nullptr);

	gatt_db_service_set_active(service, true);
	return 0;
}

int MeteoGattClientV1::buildGattService()
{
	bt_uuid_t uuid;
	bt_uuid16_create(&uuid, kUuidGatt);

	gatt_db_attribute* service = gatt_db_add_service(m_db, &uuid, true, 4);

	bt_uuid16_create(&uuid, GATT_CHARAC_SERVICE_CHANGED);
	gatt_db_service_add_characteristic(service, &uuid, BT_ATT_PERM_READ,
		BT_GATT_CHRC_PROP_READ | BT_GATT_CHRC_PROP_INDICATE,
		GattClient_onServiceChanged, nullptr, this);

	bt_uuid16_create(&uuid, GATT_CLIENT_CHARAC_CFG_UUID);
	gatt_db_service_add_descriptor(service, &uuid, BT_ATT_PERM_READ | BT_ATT_PERM_WRITE,
		GattClient_onServiceChangedRead, GattClient_onServiceChangedWrite, this);

	gatt_db_service_set_active(service, true);
	return 0;
}

int MeteoGattClientV1::buildDeviceInfoService(std::map<std::string, std::function<std::string()>> deviceInfoGetter)
{
	bt_uuid_t uuid;
	bt_uuid16_create(&uuid, kUuidDeviceInfoService);

	// TODO: I don't know what the end value is for. The last call here to add
	// the manufacturer name was failing, but when I upp'ed it from 14 to 30
	// the error went away. Not sure what's going on?
	gatt_db_attribute* service = gatt_db_add_service(m_db, &uuid, true, 30);

	addDeviceInfoCharacteristic(service, kUuidSystemId, deviceInfoGetter["GetSystemId"]());
	addDeviceInfoCharacteristic(service, kUuidModelNumber, deviceInfoGetter["GetModelNumber"]());
	addDeviceInfoCharacteristic(service, kUuidSerialNumber, deviceInfoGetter["GetSerialNumber"]());
	addDeviceInfoCharacteristic(service, kUuidFirmwareRevision, deviceInfoGetter["GetFirmwareRevision"]());
	addDeviceInfoCharacteristic(service, kUuidHardwareRevision, deviceInfoGetter["GetHardwareRevision"]());
	addDeviceInfoCharacteristic(service, kUuidSoftwareRevision, deviceInfoGetter["GetSoftwareRevision"]());
	addDeviceInfoCharacteristic(service, kUuidManufacturerName, deviceInfoGetter["GetManufacturerName"]());

	gatt_db_service_set_active(service, true);
	return 0;
}

int MeteoGattClientV1::buildMeteoService()
{
	bt_uuid_t uuid;

	bt_string_to_uuid(&uuid, kUuidRpcService.c_str());
	gatt_db_attribute* service = gatt_db_add_service(m_db, &uuid, true, 25);
	if (!service)
	{
		LOG(LogLevel::Error) << "failed to add meteo service to gatt dbr";
	}

	// data channel
	bt_string_to_uuid(&uuid, kUuidRpcInbox.c_str());
	m_data_channel = gatt_db_service_add_characteristic(
		service,
		&uuid,
		BT_ATT_PERM_READ | BT_ATT_PERM_WRITE,
		BT_GATT_CHRC_PROP_READ | BT_GATT_CHRC_PROP_WRITE,
		&GattClient_onDataChannelOut,
		&GattClient_onDataChannelIn,
		this);

	if (!m_data_channel)
	{
		LOG(LogLevel::Error) << "failed to create inbox characteristic";
	}

	// blepoll
	bt_string_to_uuid(&uuid, kUuidRpcEPoll.c_str());
	m_blepoll = gatt_db_service_add_characteristic(
		service,
		&uuid,
		BT_ATT_PERM_READ,
		BT_GATT_CHRC_PROP_READ | BT_GATT_CHRC_PROP_NOTIFY,
		&GattClient_onEPollRead,
		nullptr,
		this);
	m_notify_handle = gatt_db_attribute_get_handle(m_blepoll);

	bt_uuid16_create(&uuid, GATT_CLIENT_CHARAC_CFG_UUID);
	gatt_db_service_add_descriptor(
		service,
		&uuid,
		BT_ATT_PERM_READ | BT_ATT_PERM_WRITE,
		&GattClient_onEPollRead,
		nullptr,
		this);

	if (!m_blepoll)
	{
		LOG(LogLevel::Error) << "failed to create ble poll indicator characteristic";
	}

	gatt_db_service_set_active(service, true);
	return 0;
}

void MeteoGattClientV1::addDeviceInfoCharacteristic(gatt_db_attribute * service, uint16_t id, std::string const & value)
{
	bt_uuid_t uuid;
	bt_uuid16_create(&uuid, id);

	gatt_db_attribute* attr = gatt_db_service_add_characteristic(service, &uuid, BT_ATT_PERM_READ,
		BT_GATT_CHRC_PROP_READ, nullptr, nullptr, this);

	if (!attr)
	{
		LOG(LogLevel::Error) << "Failed to create DIS characteristic " << id;
		return;
	}

	uint8_t const* p = reinterpret_cast<uint8_t const *>(value.c_str());

	LOG(LogLevel::Info) << "setting DIS attr:" << id << " to " << value;

	// I'm not sure whether i like this or just having callbacks setup for reads
	gatt_db_attribute_write(attr, 0, p, value.length(), BT_ATT_OP_WRITE_REQ, nullptr, &DIS_writeCallback, nullptr);
}

void MeteoGattClientV1::onDataChannelOut(gatt_db_attribute * attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att * att)
{
	LOG(LogLevel::Info) << "onDataChannelOut(id=" << id << ", offset=" << offset << ", opcode=" << opcode << ")";

	static int32_t const kBufferSize = 256;
	static uint8_t buff[kBufferSize];

	int n = 0;

	if (offset == 0)
	{
		memset(buff, 0, sizeof(buff));
		n = m_outgoing_queue.GetLine((char *)buff, kBufferSize);
	}
	else
	{
		int bytesToWrite = 0;
		while ((bytesToWrite + offset) < kBufferSize && (buff[offset + bytesToWrite] != '\0'))
			bytesToWrite++;

		n = bytesToWrite;
		LOG(LogLevel::Info) << "bytesToWrite:" << bytesToWrite << " offset:" << offset << " n:" << n;
	}

	uint8_t const* value = nullptr;
	if (n > 0)
		value = buff + offset;
	else
		buff[0] = '\0';

	// printf("write:%.*s\n", n, (char *) value);
	gatt_db_attribute_read_result(attr, id, 0, value, n);
}

void MeteoGattClientV1::onDataChannelIn(gatt_db_attribute * attr, uint32_t id, uint16_t offset, uint8_t const * data, size_t len, uint8_t opcode, bt_att * att)
{
	LOG(LogLevel::Info) << "onDataChannelIn(offset=" << offset << ", len=" << len << ")";

	// TODO: should this use memory_stream?
	for (size_t i = 0; i < len; ++i)
	{
		char c = static_cast<char>(data[i + offset]);
		m_incoming_buff.push_back(c);

		if (c == kRecordDelimiter)
		{
			if (!m_data_handler)
			{
				// TODO:
				LOG(LogLevel::Warning) << "no data handler registered";
			}
			else
			{
				m_incoming_buff.push_back('\0');
				m_data_handler(&m_incoming_buff[0], m_incoming_buff.size());
			}
			m_incoming_buff.clear();
		}
	}

	gatt_db_attribute_write_result(attr, id, 0);
}

void MeteoGattClientV1::onTimeout()
{
	uint32_t bytes_available = m_outgoing_queue.size();

	if (bytes_available > 0)
	{
		LOG(LogLevel::Info) << "notification of " << bytes_available << " bytes available";

		bytes_available = htonl(bytes_available);

		//uint16_t handle = gatt_db_attribute_get_handle(m_blepoll);
		uint16_t handle = m_notify_handle;
		int ret = bt_gatt_server_send_notification(
			m_server,
			handle,
			reinterpret_cast<uint8_t *>(&bytes_available),
			sizeof(uint32_t));

		if (!ret)
		{
			LOG(LogLevel::Warning) << "failed to send notification:" << ret << " with " << bytes_available << " bytes pending";
		}
	}

	mainloop_modify_timeout(m_timeout_id, 1000);
}

void MeteoGattClientV1::onEPollRead(gatt_db_attribute * attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att * att)
{
	uint32_t value = m_outgoing_queue.size();

	LOG(LogLevel::Debug) << "onEPollRead(offset=" << offset << ", opcode=" << opcode << ")";

	value = htonl(value);
	gatt_db_attribute_read_result(attr, id, 0, reinterpret_cast<uint8_t const *>(&value),
		sizeof(value));
}

void MeteoGattClientV1::onClientDisconnected(int err)
{
	// TODO: we should stash the remote client as a member of the
	// GattClient so we can print out mac addres of client that
	// just disconnected
	LOG(LogLevel::Info) << "disconnect:" << err;
	mainloop_quit();
}

void MeteoGattClientV1::onGapRead(gatt_db_attribute * attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att * att)
{
	LOG(LogLevel::Debug) << "onGapRead " << id;

	uint8_t error = 0;
	size_t len = strlen(ServerName);
	if (offset > len)
	{
		error = BT_ATT_ERROR_INVALID_OFFSET;
	}
	else
	{
		error = 0;
		len -= offset;
		uint8_t const* value = nullptr;
		if (len)
		{
			value = reinterpret_cast<uint8_t const *>(ServerName + offset);
		}
		gatt_db_attribute_read_result(attr, id, error, value, len);
	}
}

void MeteoGattClientV1::onGapWrite(gatt_db_attribute * attr, uint32_t id, uint16_t offset, uint8_t const * data, size_t len, uint8_t opcode, bt_att * att)
{
	LOG(LogLevel::Debug) << "onGapWrite";

	uint8_t error = 0;
	if ((offset + len) == 0)
	{
		memset(ServerName, 0, sizeof(ServerName));
	}
	else if (offset > sizeof(ServerName))
	{
		error = BT_ATT_ERROR_INVALID_OFFSET;
	}
	else
	{
		memcpy(ServerName + offset, data, len);
	}

	gatt_db_attribute_write_result(attr, id, error);
}

void MeteoGattClientV1::onServiceChanged(gatt_db_attribute * attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att * att)
{
	LOG(LogLevel::Debug) << "onServiceChanged";
	gatt_db_attribute_read_result(attr, id, 0, nullptr, 0);
}

void MeteoGattClientV1::onServiceChangedRead(gatt_db_attribute * attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att * att)
{
	LOG(LogLevel::Debug) << "onServiceChangedRead";

	uint8_t value[2]{ 0x00, 0x00 };
	if (m_service_change_enabled)
		value[0] = 0x02;
	gatt_db_attribute_read_result(attr, id, 0, value, sizeof(value));
}

void MeteoGattClientV1::onServiceChangedWrite(gatt_db_attribute * attr, uint32_t id, uint16_t offset, uint8_t const * value, size_t len, uint8_t opcode, bt_att * att)
{
	LOG(LogLevel::Debug) << "onServiceChangeWrite";

	uint8_t ecode = 0;
	if (!value || (len != 2))
		ecode = BT_ATT_ERROR_INVALID_ATTRIBUTE_VALUE_LEN;

	if (!ecode && offset)
		ecode = BT_ATT_ERROR_INVALID_OFFSET;

	if (!ecode)
	{
		if (value[0] == 0x00)
			m_service_change_enabled = false;
		else if (value[0] == 0x02)
			m_service_change_enabled = true;
		else
			ecode = 0x80;
	}

	gatt_db_attribute_write_result(attr, id, ecode);
}

void MeteoGattClientV1::onGapExtendedPropertiesRead(gatt_db_attribute * attrib, uint32_t id, uint16_t offset, uint8_t opcode, bt_att * att)
{
	uint8_t value[2];
	value[0] = BT_GATT_CHRC_EXT_PROP_RELIABLE_WRITE;
	value[1] = 0;
	gatt_db_attribute_read_result(attr, id, 0, value, sizeof(value));
}
