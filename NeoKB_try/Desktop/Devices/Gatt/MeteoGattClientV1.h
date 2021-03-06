#ifndef METEO_GATT_CLIENT_V1_H
#define METEO_GATT_CLIENT_V1_H


#include "GattClient.h"
#include <thread>
#include <vector>
#include "../../../Util/MemoryStream.h"

#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>


extern "C"
{
#include <lib/uuid.h>
#include <src/shared/att.h>
#include <src/shared/gatt-db.h>
#include <src/shared/gatt-server.h>
}


using namespace Util;



namespace Desktop {
namespace Devices{
namespace Gatt {

	struct DeviceInfoProvider
	{
		std::function< std::string() > GetSystemId;
		std::function< std::string() > GetModelNumber;
		std::function< std::string() > GetSerialNumber;
		std::function< std::string() > GetFirmwareRevision;
		std::function< std::string() > GetHardwareRevision;
		std::function< std::string() > GetSoftwareRevision;
		std::function< std::string() > GetManufacturerName;
	};




	class MeteoGattClientV1 : public GattClient {

		char ServerName[64]                     {"TheUnknownServer"};
		char const      kRecordDelimiter        {30};

		uint16_t const kUuidDeviceInfoService   {0x180a};
		static const uint16_t kUuidGap          {0x1800};
		static const uint16_t kUuidGatt         {0x1801};

		uint16_t const kUuidSystemId            {0x2a23};
		uint16_t const kUuidModelNumber         {0x2a24};
		uint16_t const kUuidSerialNumber        {0x2a25};
		uint16_t const kUuidFirmwareRevision    {0x2a26};
		uint16_t const kUuidHardwareRevision    {0x2a27};
		uint16_t const kUuidSoftwareRevision    {0x2a28};
		uint16_t const kUuidManufacturerName    {0x2a29};

		std::string const kUuidRpcService       {"503553ca-eb90-11e8-ac5b-bb7e434023e8"};
		std::string const kUuidRpcInbox         {"510c87c8-eb90-11e8-b3dc-17292c2ecc2d"};
		std::string const kUuidRpcEPoll         {"5140f882-eb90-11e8-a835-13d2bd922d3f"};

	public:

		MeteoGattClientV1(int fd);

		~MeteoGattClientV1();

		virtual void Init(std::map<std::string, std::function<std::string()>> deviceInfoGetter);

		virtual void EnqueueForSend(char const* buff, int n);

		virtual void Run();

		virtual void SetDataHandler(std::function<void(char const*, int)> dHandler);

		/*--------------------callbacks--------------------*/

		void onDataChannelOut(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att);

		void onDataChannelIn(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t const* data, size_t len, uint8_t opcode, bt_att* att);

		void onTimeout();

		void onEPollRead(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att);

		void onClientDisconnected(int err);

		void onGapRead(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att);

		void onGapWrite(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t const* data, size_t len, uint8_t opcode, bt_att* att);

		void onServiceChanged(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att);

		void onServiceChangedRead(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att);

		void onServiceChangedWrite(gatt_db_attribute* attr, uint32_t id, uint16_t offset, uint8_t const* value, size_t len, uint8_t opcode, bt_att* att);

		void onGapExtendedPropertiesRead(gatt_db_attribute *attrib, uint32_t id, uint16_t offset, uint8_t opcode, bt_att* att);




	protected:

		int                 m_fd;
		bt_att*             m_att;
		gatt_db*            m_db;
		bt_gatt_server*     m_server;
		uint16_t            m_mtu;
		MemoryStream		m_outgoing_queue;
		std::vector<char>		m_incoming_buff;
		gatt_db_attribute*  m_data_channel;
		gatt_db_attribute*  m_blepoll;
		uint16_t            m_notify_handle;
		bool                m_service_change_enabled;
		int                 m_timeout_id;
		std::thread::id			m_mainloop_thread;
		std::function<void(char const* buff, int n)>      m_data_handler;


		int buildService(std::map<std::string, std::function<std::string()>> deviceInfoGetter);

		int buildGapService();

		int buildGattService();

		int buildDeviceInfoService(std::map<std::string, std::function<std::string()>> deviceInfoGetter);

		int buildMeteoService();

		/* ------------------一些工具------------------ */
		void addDeviceInfoCharacteristic(gatt_db_attribute* service, uint16_t id, std::string const& value);

		//void writeCallback(gatt_db_attribute* UNUSED_PARAM(attr), int err, void* UNUSED_PARAM(argp));

		//void throw_errno(int err, char const* fmt, ...)__attribute__((format(printf, 2, 3)));
		/* ------------------一些工具------------------ */

		

	};


}
}}







#endif