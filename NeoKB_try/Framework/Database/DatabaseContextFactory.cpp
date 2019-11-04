#include "DatabaseContextFactory.h"

#include "../Host/GameHost.h"

using namespace Framework::Database;



DatabaseContextFactory::DatabaseContextFactory(GameHost * h)
{
	host = h;
	createDatabaseContext();
}

DatabaseContext * DatabaseContextFactory::GetContext()
{
	return dbContext;
}

int DatabaseContextFactory::createDatabaseContext()
{
	dbContext = new DatabaseContext(host->GetStorage());
	return 0;
}
