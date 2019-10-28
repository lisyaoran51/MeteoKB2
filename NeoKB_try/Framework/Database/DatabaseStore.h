#ifndef DATABASE_STORE_H
#define DATABASE_STORE_H


#include "../Allocation/Hierachal/MtoObject.h"
#include "../IO/Storage.h"
#include <functional>
#include "DatabaseContext.h"


using namespace Framework::Allocation::Hierachal;
using namespace Framework::IO;
using namespace std;



namespace Framework {
namespace Database {


	/// <summary>
	/// 連結到資料庫的store，因為底下資料庫不知道怎麼接，先用strategy pattern代替掉資料庫功能
	/// </summary>
	class DatabaseStore : public MtoObject {

		DatabaseStore(function<DatabaseContext*(void)> getContext, Storage* s = nullptr);

	public:



	};


}}






#endif