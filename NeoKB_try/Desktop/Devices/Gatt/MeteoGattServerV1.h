#ifndef METEO_GATT_SERVER_V1_H
#define METEO_GATT_SERVER_V1_H


#include "GattServer.h"
#include <queue>
#include <utility>



using namespace std;


// ½Ð°Ñ¦Òbleconfd



namespace Desktop {
namespace Devices{
namespace Gatt {


	class MeteoGattServerV1 : public GattServer {



	public:

		void EnqueueAsyncMessage(const char* buff, int n);

		void OnIncomingMessage(const char* buff, int n);

	protected:

		queue<pair<char*, int>>                 m_incoming_queue;


		virtual GattListener* createListener();

		virtual GattClient* accept();

		string getSystemId();

		string getModelNumber();

		string getSerialNumber();

		string getFirmwareRevision();

		string getHardwareRevision();

		string getSoftwareRevision();;

		string getManufacturerName();



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
			void updateDeviceName(string const& name);

			/**
			 * send cmd to device by hci tool
			 * @param dev_id  the device id
			 * @param args the cmd args
			 */
			void hcitoolCmd(int dev_id, vector <string> const& args);

			/**
			 * parse hci tool string args to vector args
			 * @param str the string args
			 * @return  the vector args
			 */
			vector<string> parseArgs(string str);



		public:


			virtual int Init(vector<string> config);

			virtual GattClient* Accept(map<string, function<string()>> deviceInfoGetter);

		protected:

			int startBeacon(string name, int id);

		};

	};


}
}
}






#endif