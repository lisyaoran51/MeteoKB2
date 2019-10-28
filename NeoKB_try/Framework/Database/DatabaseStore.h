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
	/// �s�����Ʈw��store�A�]�����U��Ʈw�����D��򱵡A����strategy pattern�N������Ʈw�\��
	/// </summary>
	class DatabaseStore : public MtoObject {

		DatabaseStore(function<DatabaseContext*(void)> getContext, Storage* s = nullptr);

	public:



	};


}}






#endif