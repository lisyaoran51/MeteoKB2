#ifndef GATT_SERVER_H
#define GATT_SERVER_H

#include "GattClient.h"
#include <map>
#include <thread>
#include <vector>
#include <mutex>




namespace Desktop {
namespace Devices{
namespace Gatt{

	class GattServer {

	public:

		GattClient* Listen();

		int Run(GattClient* gClient);

		//virtual int OnIncomingMessage(const char* buff, int n);

		

		
	protected:
		
		class GattListener;

		GattListener*						gattListener = nullptr;

		GattClient*							m_client = nullptr;

		std::mutex								m_mutex;
		
		// 結束server前把資料送光，可能用不到
		std::thread*								m_dispatch_thread = nullptr;
		
		bool                                m_running;

		virtual GattListener* createListener() = 0;

		virtual GattClient* accept() = 0;



		class GattListener {

		public:

			virtual int Init(std::vector<std::string> config) = 0;

			virtual GattClient* Accept(std::map<std::string, std::function<std::string()>> deviceInfoGetter) = 0;

		protected:

			int             m_listen_fd;

			// 這個沒有用到
			//bdaddr_t        m_local_interface;

		};

	};


}}}





#endif