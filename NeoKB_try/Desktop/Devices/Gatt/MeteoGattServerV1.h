#ifndef METEO_GATT_SERVER_V1_H
#define METEO_GATT_SERVER_V1_H


#include "GattServer.h"
#include <queue>
#include <utility>





// ½Ð°Ñ¦Òbleconfd



namespace Desktop {
namespace Devices{
namespace Gatt {


	class MeteoGattServerV1 : public GattServer {



	public:

		void EnqueueAsyncMessage(const char* buff, int n);

		void OnIncomingMessage(const char* buff, int n);

	protected:

		std::queue<std::pair<char*, int>>                 m_incoming_queue;


		virtual GattListener* createListener();

		virtual GattClient* accept();

		std::string getSystemId();

		std::string getModelNumber();

		std::string getSerialNumber();

		std::string getFirmwareRevision();

		std::string getHardwareRevision();

		std::string getSoftwareRevision();;

		std::string getManufacturerName();



		class MeteoGattListenerV1 : public GattListener {

			/**
			 * Stop HCI device
			 * @param ctl the device controller(fd)
			 * @param hdev the device id
			 */
			void cmdDown(int ctl, int hdev);

			/**
			 * Start HCI device
			 * @param ctl the device controller(fd)
			 * @param hdev the device id
			 */
			void cmdUp(int ctl, int hdev);

			/**
			 * send no lead v cmd to device
			 * @param hdev the device id
			 */
			void cmdNoleadv(int hdev);

			/**
			 * send lead v to device
			 * @param hdev  the device id
			 */
			void cmdLeadv(int hdev);

			/**
			 * dump hex to console
			 * @param width the hex width
			 * @param buf the hex buffer
			 * @param len the buffer length
			 */
			void hex_dump(int width, unsigned char* buf, int len);

			/**
			 * update device name, not sure why it not worked as expected ???
			 * keep it for now
			 * @param hdev the device id
			 * @param deviceName  the device name
			 */
			void cmdName(int hdev, char const* deviceName);

			string getDeviceNameFromFile();

			/**
			 * update device name, it need restart ble service
			 * NOTE: this will run system command, and this need root permission
			 * sudo sh -c "echo 'PRETTY_HOSTNAME=tc-device' > /etc/machine-info"
			 * sudo service bluetooth restart
			 * @param name the new device name
			 */
			void updateDeviceName(std::string const& name);

			/**
			 * send cmd to device by hci tool
			 * @param dev_id  the device id
			 * @param args the cmd args
			 */
			void hcitoolCmd(int dev_id, std::vector<std::string> const& args);

			/**
			 * parse hci tool string args to vector args
			 * @param str the string args
			 * @return  the vector args
			 */
			std::vector<std::string> parseArgs(std::string str);



		public:


			virtual int Init(std::vector<std::string> config);

			virtual GattClient* Accept(std::map<std::string, std::function<std::string()>> deviceInfoGetter);

		protected:

			int startBeacon(std::string name, int id);

		};

	};


}
}
}






#endif