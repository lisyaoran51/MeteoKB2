#include "DatabaseStore.h"



using namespace Framework::Database;



DatabaseStore::DatabaseStore(function<DatabaseContext*(void)> gContext, Storage * s): RegisterType("DatabaseStore")
{
	getContext = gContext;
	storage = s;
}
