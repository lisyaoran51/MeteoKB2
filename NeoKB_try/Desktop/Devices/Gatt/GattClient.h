#ifndef GATT_CLIENT_H
#define GATT_CLIENT_H


#include <functional>
#include <map>


using namespace std;


namespace Desktop {
namespace Devices{
namespace Gatt {


	class GattClient {

	public:

		virtual void Init(map<string, function<string()>> deviceInfoGetter) = 0;

		virtual void EnqueueForSend(char const* buff, int n) = 0;

		virtual void Run() = 0;

		virtual void SetDataHandler(function<void(char const* buff, int n)> dHandler) = 0;


	};


}
}}







#endif