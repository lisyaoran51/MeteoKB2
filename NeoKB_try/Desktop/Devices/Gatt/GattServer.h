#ifndef GATT_SERVER_H
#define GATT_SERVER_H

#include "GattClient.h"
#include <map>
#include <thread>
#include <vector>
#include <mutex>


using namespace std;


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

		mutex								m_mutex;
		
		// ����server�e���ưe���A�i��Τ���
		thread*								m_dispatch_thread = nullptr;
		
		bool                                m_running;

		virtual GattListener* createListener() = 0;

		virtual GattClient* accept() = 0;



		class GattListener {

		public:

			virtual int Init(vector<string> config) = 0;

			virtual GattClient* Accept(map<string, function<string()>> deviceInfoGetter) = 0;

		protected:

			int             m_listen_fd;

			// �o�ӨS���Ψ�
			//bdaddr_t        m_local_interface;

		};

	};


}}}





#endif