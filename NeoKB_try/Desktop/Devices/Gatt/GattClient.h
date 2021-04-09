#ifndef GATT_CLIENT_H
#define GATT_CLIENT_H


#include <functional>
#include <map>




namespace Desktop {
namespace Devices{
namespace Gatt {


	class GattClient {

	public:

		virtual void Init(std::map<std::string, std::function<std::string()>> deviceInfoGetter) = 0;

		virtual void EnqueueForSend(char const* buff, int n) = 0;

		virtual void Run() = 0;

		virtual void SetDataHandler(std::function<void(char const*, int)> dHandler) = 0;

		virtual int SendNotification(char* bufferOut, int size) = 0;

		virtual int GetWriteQueueLength() = 0;

	};


}
}}







#endif